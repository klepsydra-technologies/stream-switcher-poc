#include <iostream>
#include <thread>
#include "opencv2/core/core.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>

#include "config.h"
#include "simple_write_service.h"

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  int imgWidth = 320;
  int imgHeight = 240;
  int numPublishers;
  std::string imgTopic;
  std::string imgUrlRoot;
  std::string fileImagesPathRoot;
  std::vector<std::string> fileImagesPaths;
  std::vector<std::string> imgUrls;
  int loopPeriod;

  yamlEnv.getPropertyInt("num_publishers", numPublishers);
  yamlEnv.getPropertyString("file_images_path_root", fileImagesPathRoot);
  fileImagesPathRoot = PROJECT_PATH + fileImagesPathRoot;
  spdlog::warn("fileImagesPathRoot: {}", fileImagesPathRoot);
  yamlEnv.getPropertyString("img_url_root", imgUrlRoot);
  yamlEnv.getPropertyString("image_topic", imgTopic);
  yamlEnv.getPropertyInt("loop_period", loopPeriod);
  
  zmq::context_t context(1);
  std::vector<zmq::socket_t> publishers(numPublishers);
  std::vector<kpsr::zmq_mdlw::ToZMQMiddlewareProvider> toZMQMiddlewareProviders(numPublishers);
  std::vector<kpsr::Publisher<kpsr::vision_ocv::ImageData>> toZMQPublishers(numPublishers);
  std::vector<ssw::SimpleWriteService> simpleWriteServices(numPublishers);

  for(int i=0;i<numPublishers; i++){
    fileImagesPaths[i] = fileImagesPathRoot + std::to_string(i);
    imgUrls[i] = imgUrlRoot + std::to_string(i);
    publishers[i] = zmq::socket_t(context, ZMQ_PUB);
    publishers[i].bind(imgUrls[i]);

    toZMQMiddlewareProviders[i] = kpsr::zmq_mdlw::ToZMQMiddlewareProvider(nullptr, *publishers[i]);
    toZMQPublishers[i] = toZMQMiddlewareProviders[i]->getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(imgTopic, 0);
    
    simpleWriteServices[i](nullptr, toZMQPublishers[i], fileImagesPaths[i], true);
    simpleWriteServices[i](nullptr, nullptr, fileImagesPaths[i], true);
    simpleWriteServices[i].startup();
  }

  while (true)
  {     //  --> use eventLoop scheduler  (event_loop_middleware_provider.h)
    for(int i=0;i<numPublishers; i++){
      simpleWriteServices[i].runOnce();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(loopPeriod));
  }

  for(int i=0;i<numPublishers; i++){
      simpleWriteServices[i].shutdown();
  }

  return 0;
}
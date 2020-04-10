

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
  std::string ImgUrl;
  std::string imgTopic;
  std::string fileImagesPath;
  yamlEnv.getPropertyString("img_url", ImgUrl);
  yamlEnv.getPropertyString("image_topic", imgTopic);
  yamlEnv.getPropertyString("file_images_path", fileImagesPath);

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageSubscriber;
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imagePublisher;

  zmq::context_t context (1);
  zmq::socket_t publisher (context, ZMQ_PUB);
  publisher.bind(ImgUrl);

  kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(nullptr, publisher);
  kpsr::Publisher<kpsr::vision_ocv::ImageData> * toZMQPublisher = toZMQMiddlewareProvider.getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(imgTopic, 0);

  SimpleWriteService imgPublisher(nullptr, toZMQPublisher, fileImagesPath, true);

  imgPublisher.startup();

  while (true)
  {
    spdlog::info("runOnce();");
    imgPublisher.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  imgPublisher.shutdown();

  return 0;
} // end main()







 
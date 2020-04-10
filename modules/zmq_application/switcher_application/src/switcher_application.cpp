

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
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>

#include "config.h"
#include "simple_read_service.h"
#include "stream_switcher_svc.h"
#include "display_svc.h"

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  int imgWidth = 320;
  int imgHeight = 240;
  std::string imgTopic;
  std::string imgUrlRoot;
  std::vector<std::string> recvImgUrls;
  int numSubscribers;
  int loopPeriod;

  yamlEnv.getPropertyString("recv_img_url_root", imgUrlRoot);
  yamlEnv.getPropertyString("image_topic", imgTopic);
  yamlEnv.getPropertyInt("num_subscribers", numSubscribers);
  yamlEnv.getPropertyInt("loop_period", loopPeriod);

  std::vector<kpsr::Subscriber<kpsr::vision_ocv::ImageData>*> imageSubscribers;
  std::vector<kpsr::Publisher<kpsr::vision_ocv::ImageData>*> imagePublishers;
  kpsr::vision_ocv::ImageDataFactory factory(imgWidth, imgHeight, 0, "frame");

  zmq::context_t context(1);
  std::vector<zmq::socket_t> subscribers(numSubscribers);
  std::vector<kpsr::zmq_mdlw::FromZmqChannel<Base>> binaryFromZMQProviders(numSubscribers);
  std::vector<kpsr::zmq_mdlw::FromZmqMiddlewareProvider> fromZmqMiddlewareProviders(numSubscribers);
  std::vector<ssw::SimpleReadService> simpleReadServices(numSubscribers);
  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(nullptr);

  eventloop.start();

  for(int i=0;i<numSubscribers; i++){
    recvImgUrls[i] = imgUrlRoot + std::to_string(i);
    subscribers[i] = zmq::socket_t(context, ZMQ_SUB);
    subscribers[i].connect(recvImgUrls[i]);
    subscribers[i].setsockopt(ZMQ_SUBSCRIBE, imgTopic.c_str(), imgTopic.size());

    binaryFromZMQProviders[i] = fromZmqMiddlewareProviders[i].getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(subscribers[i], 128);
    binaryFromZMQProviders[i].start();
    imageSubscribers[i] = eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(imgTopic);
    imagePublishers[i] = eventloop.getPublisher<kpsr::vision_ocv::ImageData>(imgTopic, EVENT_LOOP_SIZE, factory.initializerFunction, factory.eventClonerFunction);
    binaryFromZMQProviders[i].registerToTopic(imgTopic, imagePublishers[i]);

    simpleReadServices[i](nullptr, *imageSubscribers[i]);
    simpleReadServices[i].startup();
  }

  ssw::StreamSwitcherSvc streamSwitcherSvc(&yamlEnv);
  ssw::DisplaySvc displaySvc(&yamlEnv, imageSubscribers[0]);

  while (true)
  {     //  --> use eventLoop scheduler  (event_loop_middleware_provider.h)

    for(int i=0;i<numSubscribers; i++){
      simpleReadServices[i].runOnce();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(loopPeriod));
  }

  for(int i=0;i<numSubscribers; i++){
      simpleReadServices[i].shutdown();
  }

  return 0;
}


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
  std::string recvImgUrl;
  std::string imgTopic;
  yamlEnv.getPropertyString("recv_img_url", recvImgUrl);
  yamlEnv.getPropertyString("image_topic", imgTopic);

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageSubscriber;
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imagePublisher;

  zmq::context_t context(1);
  zmq::socket_t subscriber(context, ZMQ_SUB);
  kpsr::zmq_mdlw::FromZmqChannel<Base> *binaryFromZMQProvider;
  kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
  kpsr::high_performance::EventLoopMiddlewareProvider<128> eventLoop(nullptr);
  kpsr::vision_ocv::ImageDataFactory factory(imgWidth, imgHeight, 0, "frame");

  eventLoop.start();
  subscriber.connect(recvImgUrl);
  subscriber.setsockopt(ZMQ_SUBSCRIBE, imgTopic.c_str(), imgTopic.size());
  binaryFromZMQProvider =
      fromZmqMiddlewareProvider
          .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(
              subscriber, 128);

  binaryFromZMQProvider->start();
  imageSubscriber =
      eventLoop.getSubscriber<kpsr::vision_ocv::ImageData>(imgTopic);
  imagePublisher =
      eventLoop.getPublisher<kpsr::vision_ocv::ImageData>(imgTopic, EVENT_LOOP_SIZE, factory.initializerFunction, factory.eventClonerFunction);

  binaryFromZMQProvider->registerToTopic(imgTopic, imagePublisher);
 
  SimpleReadService imgSubscriber(nullptr, imageSubscriber);

  imgSubscriber.startup();

  ssw::StreamSwitcherSvc streamSwitcherSvc(&yamlEnv);
  streamSwitcherSvc.startup();

  ssw::DisplaySvc displaySvc(&yamlEnv);
  displaySvc.startup();

  while (true)
  {
    imgSubscriber.runOnce();
    //streamSwitcherSvc.runOnce();
    //displaySvc.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  displaySvc.shutdown();
  streamSwitcherSvc.shutdown();
  imgSubscriber.shutdown();

  return 0;
} // end main()







 
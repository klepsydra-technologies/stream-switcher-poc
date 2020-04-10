#include <iostream>
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>
#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>

#include "config.h"
#include "simple_read_service.h"

void showImage(kpsr::vision_ocv::ImageData &image){
    spdlog::info("Showing Image {}", image.seq);
    if(image.img.empty())
    {
        spdlog::info("Could not read the image");
        return;
    }
    cv::imshow("Show image", image.img);
}

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);
  int imgWidth = 320;
  int imgHeight = 240;
  std::string imgUrl;
  std::string imgTopic;
  yamlEnv.getPropertyString("img_url", imgUrl);
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
  subscriber.connect(imgUrl);
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

  while (true)
  {
    imgSubscriber.runOnce();
    showImage(imgSubscriber._lastReadImg);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  imgSubscriber.shutdown();

  return 0;
} // end main()

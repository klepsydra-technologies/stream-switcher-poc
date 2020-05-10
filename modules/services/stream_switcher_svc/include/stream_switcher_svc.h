#ifndef STREAMSWITCHERSVC_H
#define STREAMSWITCHERSVC_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <opencv2/opencv.hpp>

#include <klepsydra/core/environment.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/vision_ocv/image_event_data.h>

namespace ssw
{
class StreamSwitcherSvc : public kpsr::Service
{
public:
  StreamSwitcherSvc(kpsr::Environment *environment);

  ~StreamSwitcherSvc(){};

protected:
  void start() override;
  void stop() override;
  void execute() override;
/*
  vec<Subscriber<NCams>>
  Publisher<cam>
  subscriber<idCam>
  vector<imgdata>
  [_index, , i, _publisher](const kpsr::ImgData & image) {
*/
private:
  void onImgReceived(const kpsr::vision_ocv::ImageData &img);

  std::vector<kpsr::Subscriber<kpsr::vision_ocv::ImageData>> *_imageDataSubscribers;
  kpsr::vision_ocv::ImageData _image;
  int _activeCamera;
};
} // namespace ssw
#endif // STREAMSWITCHERSVC_H
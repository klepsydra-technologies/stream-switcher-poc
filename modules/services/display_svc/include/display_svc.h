#ifndef DISPLAY_SVC_H
#define DISPLAY_SVC_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/vision_ocv/image_event_data.h>

namespace ssw
{
class DisplaySvc : public kpsr::Service
{
public:
  DisplaySvc(kpsr::Environment *environment);

  ~DisplaySvc(){};

protected:
  void start() override;
  void stop() override;
  void execute() override;

  void showImage(kpsr::vision_ocv::ImageData &image);

private:
  kpsr::vision_ocv::ImageData _currentImage;

};
} // namespace ssw
#endif // DISPLAY_SVC_H
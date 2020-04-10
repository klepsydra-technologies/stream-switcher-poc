#include <iostream>
#include "display_svc.h"

namespace ssw
{
DisplaySvc::DisplaySvc(kpsr::Environment *environment, kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imgsubscriber)
    : Service(environment, "stream_switcher_service")
    , _activeCamera(environment, imgsubscriber) {}

void DisplaySvc::start()
{
  spdlog::info("Start display service");
}

void DisplaySvc::stop()
{
  spdlog::info("Stop display service");
}

void DisplaySvc::execute()
{ 
}

void DisplaySvc::showImage(kpsr::vision_ocv::ImageData &image)
{
  spdlog::info("Showing Image {}", image.seq);
  if (image.img.empty())
  {
    spdlog::info("Could not read the image");
    return;
  }
  cv::imshow("Show image", image.img);
  image.seq++;
}
} // namespace ssw
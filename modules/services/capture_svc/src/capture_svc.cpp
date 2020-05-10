#include "capture_svc.h"

namespace ssw
{
CaptureSvc::CaptureSvc(kpsr::Environment *environment)
    : Service(environment, "capture_service") {}

void CaptureSvc::start()
{
  spdlog::info("Start capture service");
}

void CaptureSvc::stop()
{
  spdlog::info("Stop capture service");
}

void CaptureSvc::execute()
{
  spdlog::info("Execute");
}

void CaptureSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img)
{
  spdlog::info("image received {}", img.seq);
}

} // namespace ssw

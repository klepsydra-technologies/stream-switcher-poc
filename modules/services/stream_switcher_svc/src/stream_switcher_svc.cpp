#include "stream_switcher_svc.h"

namespace ssw
{
StreamSwitcherSvc::StreamSwitcherSvc(kpsr::Environment *environment)
    : Service(environment, "stream_switcher_service") {}

void StreamSwitcherSvc::start()
{
  spdlog::info("Start stream switcher service");
}

void StreamSwitcherSvc::stop()
{
  spdlog::info("Stop stream switcher service");
}

void StreamSwitcherSvc::execute()
{
  spdlog::info("Execute");
}

void StreamSwitcherSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img)
{
  spdlog::info("image received {}", img.seq);
}

} // namespace ssw

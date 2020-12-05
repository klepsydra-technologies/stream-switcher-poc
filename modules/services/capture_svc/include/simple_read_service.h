#ifndef SIMPLE_READ_SERVICE_H
#define SIMPLE_READ_SERVICE_H

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
class SimpleReadService : public kpsr::Service
{
public:
    SimpleReadService(kpsr::Environment * environment, kpsr::Subscriber<kpsr::vision_ocv::ImageData> * subscriber)
        : kpsr::Service(environment, "SimpleReadService")
        , _subscriber(subscriber)
    {}

    void start() {
        std::function<void(kpsr::vision_ocv::ImageData)> simpleListener = std::bind(&SimpleReadService::onEventReceived, this, std::placeholders::_1);
        this->_subscriber->registerListener("SimpleReadService", simpleListener);
    }

    void stop() {
        this->_subscriber->removeListener("SimpleReadService");
    }

    void execute() {}

    void onEventReceived(const kpsr::vision_ocv::ImageData & event) {
        spdlog::info("SimpleReadService. event received. {}"
                  ". image type: {}"
                  ". image rows: {}"
                  ". image cols: {}",
                  event.seq,
                  event.img.type(),
                  event.img.rows,
                  event.img.cols
        );
        _lastReadImg = event;
        _receivedImage = true;
    }
    void displayImg() {
        if(!_lastReadImg.img.data) {
            spdlog::warn("no image data");
        }
        cv::imshow("Display window", _lastReadImg.img); // CONFIGURABLE WINDOWS_NAME
        spdlog::info("displaying..."); //OpenCV compiled with GTK_ON, but window is not displayed in Ubuntu18.04 (test OCV4)
    }

    bool _receivedImage = false;
    kpsr::vision_ocv::ImageData _lastReadImg;

private:
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> * _subscriber;
    long _startTimestamp;
};
}
#endif // SIMPLE_READ_SERVICE_H

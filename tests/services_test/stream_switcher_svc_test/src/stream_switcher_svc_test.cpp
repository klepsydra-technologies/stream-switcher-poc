
#include "stream_switcher_svc.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <klepsydra/core/cache_listener.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/event_emitter_publisher.h>
#include <klepsydra/core/event_emitter_subscriber.h>
#include <klepsydra/mem_core/mem_env.h>

TEST(StreamSwitcherSvc, TestCase1) {

  ASSERT_TRUE(1);

  spdlog::info("TestCase1 completed.");
}

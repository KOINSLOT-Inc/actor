// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unity.h"

#include <Actor.hpp>

class TestActor : public Actor::Actor {
public:
  bool initialized = false;
  const char *getName() { return "testActor"; };
  void initialize() { initialized = true; };
  void handle(::Actor::Message *message){};
};

TEST_CASE("Actor initializes", "[actor]") {
  TestActor testActor = TestActor();
  testActor.start();

  vTaskDelay(pdMS_TO_TICKS(10));

  TEST_ASSERT_EQUAL(true, testActor.initialized);
}

// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Actor.hpp"

class Greeter : public Actor::Actor {
public:
  const char *getName() { return "greeter"; };
  void initialize() {
    while (true) {
      printf("Hello, World!\n");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  };
  void handle(::Actor::Message *message){};
} greeter;

extern "C" void app_main(void) { greeter.start(); }

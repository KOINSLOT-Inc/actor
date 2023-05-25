// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Actor.hpp"

typedef enum : int {
  CALL,
} Events;

class Caller : public Actor::Actor {
public:
  const char *getName() { return "caller"; };
  void initialize() {
    while (true) {
      printf("Hello, Responder!\n");

      ::Actor::Message message(CALL);
      publish(&message);

      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  };
  void handle(::Actor::Message *message){};
} caller;

class Responder : public Actor::Actor {
public:
  const char *getName() { return "responder"; };
  void initialize() {}

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case CALL:
      printf("Oh hi there, Caller!\n");
    }
  };
} responder;

extern "C" void app_main(void) {
  responder.subscribe(&caller);

  responder.start();
  caller.start();
}

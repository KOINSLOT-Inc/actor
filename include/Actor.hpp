// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

namespace Actor {
const uint8_t MAX_SUBSCRIBERS = 5;

typedef enum : uint8_t {
  DIRECTIVE_HANDLE,
  DIRECTIVE_EXIT,
} Directive;

typedef struct Message {
  Message() : directive(DIRECTIVE_HANDLE), signal(0), data(nullptr){};
  Message(int signal)
      : directive(DIRECTIVE_HANDLE), signal(signal), data(nullptr){};
  Message(int signal, void *data)
      : directive(DIRECTIVE_HANDLE), signal(signal), data(data){};
  Directive directive;
  int signal;
  void *data;
} Message;

class Actor {
private:
  QueueHandle_t queue;

  Actor *subscribers[MAX_SUBSCRIBERS] = {};

  void static loop(void *parameters);

  virtual void handle(Message *message) = 0;

public:
  virtual const char *getName() = 0;

  void start();
  void stop();

  virtual void initialize();
  virtual void teardown();

  void dispatch(Message *message);
  void publish(Message *message);

  void subscribe(Actor *actor);
  void addSubscriber(Actor *actor);
  void unsubscribe(Actor *actor);
  void removeSubscriber(Actor *actor);

  QueueHandle_t getQueue();
};

} // namespace Actor

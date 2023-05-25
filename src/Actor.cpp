// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Actor.hpp"

namespace Actor {

void Actor::initialize(){};
void Actor::teardown(){};

void Actor::loop(void *taskInstance) {
  Actor *actor = (Actor *)taskInstance;
  actor->initialize();

  for (;;) {
    Message message;
    xQueueReceive(actor->queue, &message, portMAX_DELAY);

    switch (message.directive) {
    case DIRECTIVE_HANDLE:
      actor->handle(&message);
      break;
    case DIRECTIVE_EXIT:
      actor->teardown();
      vTaskDelete(NULL);
      break;
    default:
      break;
    }
  }
}

void Actor::start() {
  queue = xQueueCreate(10, sizeof(Message));
  xTaskCreate(loop,      // method to run
              getName(), // use actor name for task name
              10000,     // default to 10kb of stack memory
              this,      // pass in actor as parameter to method
              1,         // set priority to greater than idle task priority (0)
              NULL       // no handle needed in this case
  );
}

void Actor::dispatch(Message *message) {
  // user cannot dispatch special system directives
  message->directive = DIRECTIVE_HANDLE;

  xQueueSend(this->queue, message, portMAX_DELAY);
}

void Actor::stop() {
  Message message;
  message.directive = DIRECTIVE_EXIT;
  xQueueSend(this->queue, &message, portMAX_DELAY);
}

void Actor::addSubscriber(Actor *actor) {
  if (actor == this) {
    // TODO: error
    return; // cannot subscribe to self
  }

  uint8_t i = 0;
  while (i < MAX_SUBSCRIBERS && subscribers[i] != nullptr) {
    if (subscribers[i] == actor) {
      // TODO: error
      return; // already subscribed
    }
    i++;
  }

  if (i == MAX_SUBSCRIBERS) {
    // TODO: error
    return; // subscriber limit reached
  }

  subscribers[i] = actor;
}

void Actor::subscribe(Actor *actor) { actor->addSubscriber(this); }

void Actor::removeSubscriber(Actor *actor) {
  if (actor == this) {
    // TODO: error
    return; // cannot unsubscribe from self
  }

  uint8_t i = 0;
  bool removed = false;
  while (i < MAX_SUBSCRIBERS && subscribers[i] != nullptr) {
    if (removed) {
      // shift remaining subscribers down so that `publish` calls don't have
      // to iterate over the entire list, just the beginning populated section
      subscribers[i - 1] = subscribers[i];
      subscribers[i] = nullptr;
    } else if (subscribers[i] == actor) {
      subscribers[i] = nullptr;
      removed = true;
    }
    i++;
  }
}

void Actor::unsubscribe(Actor *actor) { actor->removeSubscriber(this); }

void Actor::publish(Message *message) {
  uint8_t i = 0;
  while (i < MAX_SUBSCRIBERS) {
    if (subscribers[i] == nullptr) {
      // reached end of subscriber list,
      // no need to iterate over rest of nullptrs in the array
      return;
    }

    subscribers[i]->dispatch(message);

    i++;
  }
}

QueueHandle_t Actor::getQueue() { return queue; }

} // namespace Actor

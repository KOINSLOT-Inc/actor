// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unity.h"

extern "C" void app_main(void) {
  UNITY_BEGIN();
  unity_run_all_tests();
  UNITY_END();
  exit(0);
}

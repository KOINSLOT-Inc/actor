# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later


def test_app(dut):
    dut.expect("Hello, Responder!")
    dut.expect("Oh hi there, Caller!")

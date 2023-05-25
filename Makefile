# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: test
test:
	docker run --rm -v $$PWD:/actor -w /actor espressif/idf:release-v5.1 \
		bash -c 'idf.py --project-dir test_app build && ./test_app/build/actor-tests.elf'

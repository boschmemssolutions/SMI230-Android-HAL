// SPDX-License-Identifier: Apache-2.0
/*
 * Copyright (C) 2021 Robert Bosch GmbH. All rights reserved. 
 * Copyright (C) 2011~2015 Bosch Sensortec GmbH All Rights Reserved
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "BoschSensor.h"
#include "sensord_cfg.h"
#include "sensord_pltf.h"
#include "sensord_hwcntl.h"
#include "util_misc.h"

int g_place_a = 0;
int g_place_m = 0;
int g_place_g = 0;
int solution_type = SOLUTION_IMU;
int accl_chip = ACC_CHIP_SMI230;
int gyro_chip = GYR_CHIP_SMI230;
int magn_chip = 0;
int accl_range = ACC_CHIP_RANGCONF_8G;
int algo_pass = 1;
int amsh_intr_pin = 0;
int amsh_calibration = 0;
int data_log = 0;
int bsx_datalog = 0;
int trace_level = 0x1C; //NOTE + ERR + WARN
int trace_to_logcat = 1;
long long unsigned int sensors_mask = 0;


void BoschSensor::sensord_cfg_init()
{

    return;
}


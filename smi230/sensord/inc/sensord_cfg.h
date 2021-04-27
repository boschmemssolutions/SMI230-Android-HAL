/* SPDX-License-Identifier: Apache-2.0 */
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

#ifndef __SENSORD_CFG_H
#define __SENSORD_CFG_H

extern int g_place_a;
extern int g_place_m;
extern int g_place_g;
extern int solution_type;
extern int accl_chip;
extern int gyro_chip;
extern int magn_chip;
extern int accl_range;
extern int algo_pass;
extern int amsh_intr_pin;
extern int amsh_calibration;
extern int data_log;
extern int bsx_datalog;
extern int trace_level;
extern int trace_to_logcat;
extern long long unsigned int sensors_mask;

//#define SMI230_DATA_SYNC

#define SOLUTION_MDOF       0
#define SOLUTION_ECOMPASS   1
#define SOLUTION_IMU        2
#define SOLUTION_M4G        3
#define SOLUTION_ACC        4
/* value is multiplexed with AP solutions, no conflicts*/
#define SOLUTION_BMA4xy_android     0
#define SOLUTION_BMA4xy_legacy      1
#define SOLUTION_BMA4xy_default     2

#define ACC_CHIP_BMI160      0
#define ACC_CHIP_BMA2x2      1
#define ACC_CHIP_SMI230      2
#define GYR_CHIP_BMI160      0
#define GYR_CHIP_BMG160      1
#define GYR_CHIP_SMI230      2
#define MAG_CHIP_BMI160      0
#define MAG_CHIP_AKM09912    1
#define MAG_CHIP_BMM150      2
#define MAG_CHIP_AKM09911    3
#define MAG_CHIP_YAS537      4
#define MAG_CHIP_YAS532      5

#define ACC_CHIP_RANGCONF_2G   2
#define ACC_CHIP_RANGCONF_4G   4
#define ACC_CHIP_RANGCONF_8G   8
#define ACC_CHIP_RANGCONF_16G  16

#endif

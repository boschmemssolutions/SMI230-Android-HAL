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

#ifndef __SENSORD_ALGO_H
#define __SENSORD_ALGO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsx_activity_bit_identifier.h"
#include "bsx_android.h"
#include "bsx_constant.h"
#include "bsx_datatypes.h"
#include "bsx_library.h"
#include "bsx_module_identifier.h"
#include "bsx_physical_sensor_identifier.h"
#include "bsx_property_set_identifier.h"
#include "bsx_return_value_identifier.h"
#include "bsx_user_def.h"
#include "bsx_vector_index_identifier.h"
#include "bsx_virtual_sensor_identifier.h"

#ifdef __cplusplus
}
#endif

#define SAMPLE_RATE_DISABLED 65535.f
#define BST_DLOG_ID_START 256
#define BST_DLOG_ID_SUBSCRIBE_OUT BST_DLOG_ID_START
#define BST_DLOG_ID_SUBSCRIBE_IN (BST_DLOG_ID_START+1)
#define BST_DLOG_ID_DOSTEP (BST_DLOG_ID_START+2)
#define BST_DLOG_ID_ABANDON (BST_DLOG_ID_START+3)
#define BST_DLOG_ID_NEWSAMPLE (BST_DLOG_ID_START+4)

extern int sensord_bsx_init(void);

extern void sensord_algo_process(BoschSensor *boschsensor);
extern bsx_return_t sensord_update_subscription(
                            bsx_sensor_configuration_t *const virtual_sensor_config_p,
                            bsx_u32_t *const n_virtual_sensor_config_p,
                            bsx_sensor_configuration_t *const physical_sensor_config_p,
                            bsx_u32_t *const n_physical_sensor_config_p,
                            uint32_t cur_active_cnt);
extern uint8_t sensord_resample5to4(int32_t data[3], int64_t *tm,  int32_t pre_data[3], int64_t *pre_tm, uint32_t counter);

#endif

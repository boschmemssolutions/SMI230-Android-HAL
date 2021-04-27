/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Robert Bosch GmbH. All rights reserved. 
 * Copyright (C) 2011~2015 Bosch Sensortec GmbH All Rights Reserved
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

/*! @addtogroup bsx
 * @{
 */

#ifndef BSX_VIRTUAL_SENSOR_IDENTIFIER_H_
#define BSX_VIRTUAL_SENSOR_IDENTIFIER_H_

/*! @brief Provides the enumeration containing virtual sensor identifier 
 *
 * @{
 */ 
#define  BSX_VIRTUAL_SENSOR_ID_INVALID   (0)
#define BSX_OUTPUT_ID_ACCELERATION_RAW   (2)
#define BSX_OUTPUT_ID_ACCELERATION_CORRECTED   (4)
#define BSX_OUTPUT_ID_ACCELERATION_OFFSET   (6)
#define BSX_WAKEUP_ID_ACCELERATION_CORRECTED   (8)
#define BSX_WAKEUP_ID_ACCELERATION_RAW   (10)
#define BSX_OUTPUT_ID_ANGULARRATE_RAW   (12)
#define BSX_OUTPUT_ID_ANGULARRATE_CORRECTED   (14)
#define BSX_OUTPUT_ID_ANGULARRATE_OFFSET   (16)
#define BSX_WAKEUP_ID_ANGULARRATE_CORRECTED   (18)
#define BSX_WAKEUP_ID_ANGULARRATE_RAW   (20)
#define BSX_OUTPUT_ID_MAGNETICFIELD_RAW   (22)
#define BSX_OUTPUT_ID_MAGNETICFIELD_CORRECTED   (24)
#define BSX_OUTPUT_ID_MAGNETICFIELD_OFFSET   (26)
#define BSX_WAKEUP_ID_MAGNETICFIELD_CORRECTED   (28)
#define BSX_WAKEUP_ID_MAGNETICFIELD_RAW   (30)
#define BSX_OUTPUT_ID_GRAVITY   (32)
#define BSX_WAKEUP_ID_GRAVITY   (34)
#define BSX_OUTPUT_ID_LINEARACCELERATION   (36)
#define BSX_WAKEUP_ID_LINEARACCELERATION   (38)
/** @brief NDOF, e-Compass or M4G */
#define BSX_OUTPUT_ID_ROTATION   (40)
/** @brief NDOF, e-Compass or M4G */
#define BSX_WAKEUP_ID_ROTATION   (42)
/** @brief no magnetic field */
#define BSX_OUTPUT_ID_ROTATION_GAME   (44)
/** @brief no magnetic field */
#define BSX_WAKEUP_ID_ROTATION_GAME   (46)
/** @brief no gyroscope, either e-Compass or M4G */
#define BSX_OUTPUT_ID_ROTATION_GEOMAGNETIC   (48)
/** @brief no gyroscope, either e-Compass or M4G */
#define BSX_WAKEUP_ID_ROTATION_GEOMAGNETIC   (50)
/** @brief deprecated: NDOF, e-Compass or M4G computed from OUTPUT_ROTATION */
#define BSX_OUTPUT_ID_ORIENTATION   (52)
/** @brief deprecated: NDOF, e-Compass or M4G computed from WAKEUP_ROTATION */
#define BSX_WAKEUP_ID_ORIENTATION   (54)
#define BSX_OUTPUT_ID_FLIP_STATUS   (62)
#define BSX_OUTPUT_ID_TILT_STATUS   (64)
#define BSX_OUTPUT_ID_STEPDETECTOR   (66)
#define BSX_OUTPUT_ID_STEPCOUNTER   (68)
#define BSX_WAKEUP_ID_STEPCOUNTER   (70)
#define BSX_OUTPUT_ID_SIGNIFICANTMOTION_STATUS   (72)
#define BSX_OUTPUT_ID_WAKE_STATUS   (74)
#define BSX_OUTPUT_ID_GLANCE_STATUS   (76)
#define BSX_OUTPUT_ID_PICKUP_STATUS   (78)
#define BSX_OUTPUT_ID_ACTIVITY   (80)
#define BSX_OUTPUT_ID_PROPAGATION   (82)
#define BSX_OUTPUT_ID_POSITION_STEPS   (84)
#define BSX_OUTPUT_ID_STANDBY_STATUS   (86)
#define BSX_OUTPUT_ID_ACCELERATION_STATUS   (90)
#define BSX_OUTPUT_ID_ACCELERATION_DYNAMIC   (92)
#define BSX_OUTPUT_ID_ANGULARRATE_STATUS   (94)
#define BSX_OUTPUT_ID_MAGNETICFIELD_STATUS   (96)
/** @brief dedicated debug output for the angular rate from M4G */
#define BSX_OUTPUT_ID_ANGULARRATE_M4G   (98)
/** @brief dedicated debug output for the angular rate from M4G */
#define BSX_WAKEUP_ID_ANGULARRATE_M4G   (100)
/*!
 * @}
 */ 

#endif /* BSX_VIRTUAL_SENSOR_IDENTIFIER_H_ */

/*! @}
 */

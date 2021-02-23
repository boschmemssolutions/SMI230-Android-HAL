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

#ifndef BSX_PROPERTY_SET_IDENTIFIER_H_
#define BSX_PROPERTY_SET_IDENTIFIER_H_

/*! @brief Provides the enumeration containing property set identifier 
 *
 * @{
 */ 
#define BSX_PROPERTY_SET_ID_FULL   (0)
#define BSX_PROPERTY_SET_ID_MINIMAL   (1)
#define BSX_PROPERTY_SET_ID_TYPICAL   (2)
/** @brief phone related values, e.g. SIC matrix, conversion factors for sensor resolution and range */
#define BSX_PROPERTY_SET_ID_CONDITIONING   (10)
#define BSX_PROPERTY_SET_ID_PREPROCESSING   (20)
#define BSX_PROPERTY_SET_ID_CALIBRATOR_ALL   (31)
/** @brief State of accel calibrator */
#define BSX_PROPERTY_SET_ID_CALIBRATOR_ACCELEROMETER   (32)
/** @brief state of gyro calibrator */
#define BSX_PROPERTY_SET_ID_CALIBRATOR_GYROSCOPE   (33)
/** @brief state of magnetometer */
#define BSX_PROPERTY_SET_ID_CALIBRATOR_MAGNETOMETER   (34)
/** @brief magnetometer related values, e.g. noise and offsets, for sensors such as BMM*, YAS*, AK* */
#define BSX_PROPERTY_SET_ID_MAGNETOMETER   (35)
/** @brief magnetometer related values, e.g. noise and offsets, for sensors such as BMM*, YAS*, AK* */
#define BSX_PROPERTY_SET_ID_MAGNETOMETER_CALIBRATION   (36)
#define BSX_PROPERTY_SET_ID_MAGNETOMETER_SOFT_IRON_EQUALIZATION   (37)
#define BSX_PROPERTY_SET_ID_MAGNETOMETER_INTERFACE   (38)
#define BSX_PROPERTY_SET_ID_DIRECTIONTRACKER_ALL   (41)
#define BSX_PROPERTY_SET_ID_DIRECTIONTRACKER_COMPASS   (42)
#define BSX_PROPERTY_SET_ID_DIRECTIONTRACKER_M4G   (43)
#define BSX_PROPERTY_SET_ID_DIRECTIONTRACKER_IMU   (44)
#define BSX_PROPERTY_SET_ID_DIRECTIONTRACKER_NDOF   (45)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_ALL   (51)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_COMPASS_HEADING_SENSITIVITY   (52)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_NDOF_ORIENTATION_CORRECTION_SPEED   (53)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_MAG_CALIB_ACC_SENSITIVITY   (54)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_MAG_CALIB_SPEED   (55)
#define BSX_PROPERTY_SET_ID_PREDEFINED_FILTERS_COMPASS_MAG_CALIB_ACC_AUTO_REC_MODE   (56)
#define BSX_PROPERTY_SET_ID_TURNOFF   (36)
#define BSX_PROPERTY_SET_ID_MIXED   (61)
#define BSX_PROPERTYSET_FULL                           BSX_PROPERTY_SET_ID_FULL
#define BSX_PROPERTYSET_CALIBRATOR_ACCELEROMETER       BSX_PROPERTY_SET_ID_CALIBRATOR_ACCELEROMETER
#define BSX_PROPERTYSET_CALIBRATOR_MAGNETOMETER        BSX_PROPERTY_SET_ID_CALIBRATOR_MAGNETOMETER
#define BSX_PROPERTYSET_CALIBRATOR_GYROSCOPE           BSX_PROPERTY_SET_ID_CALIBRATOR_GYROSCOPE
/*!
 * @}
 */ 

#endif /* BSX_PROPERTY_SET_IDENTIFIER_H_ */

/*! @}
 */

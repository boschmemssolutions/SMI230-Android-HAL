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

/*!@addtogroup bsx
 * @{*/

#ifndef __BSX_CONSTANT_H__
#define __BSX_CONSTANT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Physical constant for the average gravitational force on Earth */
#define BSX_CONSTANT_GRAVITY_STANDARD                   (9.80665f)
/*! @brief Mathematical constant for \f$ \pi \f$ */
#define BSX_CONSTANT_PI                                 (3.141592653589793f)
#define BSX_CONSTANT_PI_OVER_TWO                        (1.570796326794897f)   //!< \f$ pi/2 \f$
#define BSX_CONSTANT_PI_TIMES_TWO                       (6.283185307179586f)   //!< \f$ 2 \cdot pi \f$
#define BSX_CONSTANT_SQRT_TWO_OVER_TWO                  (0.7071067811865476f) //!< \f$ 0.5\cdot \sqrt(2)\f$
#define BSX_CONSTANT_UNIT_SCALING_RADIAN2DEGREE         (57.295779513082320876798154814105f)  //!< \f$ 180/pi \f$
#define BSX_CONSTANT_UNIT_SCALING_DEGREE2RADION         (0.01745329251994329576923690768489f)   //!< \f$ pi/180 \f$


/** @name Special values for sample rates
 *
 * @{
 */
/** @brief Special value for "disabled" signal that cannot or shall not provide any signal
 *
 * Sample rate value to define the operation mode "disabled". Not a valid sample interval for operation. */
#define BSX_SAMPLE_RATE_DISABLED       (float)(UINT16_MAX)
/** @brief Largest possible sample rate */
#define BSX_SAMPLE_RATE_MAX            (float)(UINT16_MAX - 16U)
/** @brief smallest possible sample rate */
#define BSX_SAMPLE_RATE_MIN           (1.0f)
/** @brief Sample rate value to define non-continuity of outputs of a module or a signal.
 *
 * Non-continuously sampled signals are called events. */
#define BSX_SAMPLE_RATE_EVENT         (0.0f)
/** @} */


/*! @brief conversion factors among internal number representation to physical number representation
 *
 * @note The constants provided within the following group shall be used only when the
 *       configuration applied to the fusion library does apply scaling to output values!
 *
 * @{
 */
#define BSX_CONSTANT_UNIT_SCALING_ACC_OUTPUT2G          (0.001f) //!< internal unit [mg] to [g]
#define BSX_CONSTANT_UNIT_SCALING_ACC_OUTPUT2MPS2       (BSX_CONSTANT_GRAVITY_STANDARD/1000.0f) //!< internal unit [mg] to [m/s^2]
#define BSX_CONSTANT_UNIT_SCALING_ACC_G2OUTPUT          (1000.0f) //!< [g] to internal unit [mg]
#define BSX_CONSTANT_UNIT_SCALING_ACC_MPS22OUTPUT       (1000.0f/BSX_CONSTANT_GRAVITY_STANDARD) //!< [m/s^2] to internal unit [mg]
#define BSX_CONSTANT_UNIT_SCALING_GYRO_OUTPUT2DEGPS     (0.06103515625f) //!< internal unit [0.061 deg/s] to [deg/s]; note: 2000/2^15 = 0.061
#define BSX_CONSTANT_UNIT_SCALING_GYRO_OUTPUT2RADPS     (0.001065264436031695f) //!< internal unit [0.0011 rad/s] to [deg/s]; note: 2000/2^15*pi/180 = 0.0011
#define BSX_CONSTANT_UNIT_SCALING_GYRO_DEGPS2OUTPUT     (16.384f) //!< [deg/s] to internal unit [0.061 deg/s]; note: 2^1/2000 = 16.384
#define BSX_CONSTANT_UNIT_SCALING_GYRO_RADPS2OUTPUT     (938.7340515423410f) //!< internal unit [0.0011 rad/s] to [deg/s]; note: 2000/2^15*pi/180 = 0.0011
#define BSX_CONSTANT_UNIT_SCALING_MAG_OUTPUT2UTESLA     (0.1f) //!< internal unit [0.1 uT] to [uT]
#define BSX_CONSTANT_UNIT_SCALING_MAG_UTESLA2OUTPUT     (10.0f) //!< [uT] to internal unit [0.1 uT]
/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* __BSX_CONSTANT_H__ */

/*! @}*/

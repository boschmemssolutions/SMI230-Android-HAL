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

#ifndef __BSX_ACTIVITY_BIT_IDENTIFIER_H__
#define __BSX_ACTIVITY_BIT_IDENTIFIER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief BSX activity identifiers */
typedef enum
{
    BSX_ID_ACTIVITY_STILL           = (1), /*!< Still, similar to a static state */
    BSX_ID_ACTIVITY_WALK            = (2), /*!< Walking */
    BSX_ID_ACTIVITY_RUN             = (3), /*!< Running */
    BSX_ID_ACTIVITY_BIKE            = (4), /*!< On the bicycle */
    BSX_ID_ACTIVITY_INVEHICLE       = (5), /*!< In a vehicle or on a motor-bike */
    BSX_ID_ACTIVITY_TILTING         = (6), /*!< Tilt gesture */
    BSX_ID_ACTIVITY_VEHICLESTILL    = (7), /*!< In a non-moving vehicle */
    BSX_ID_ACTIVITY_MAX             = (16) /*!< Maximum possible value for an activity */
} bsx_activity_identifier_t;

/*! @brief convert activity identifier to bit mask used to encode its begin */
#define BSX_ID_ACTIVITY_START(x) (1 << ((x)-1))

/*! @brief convert activity identifier to bit mask used to encode its end */
#define BSX_ID_ACTIVITY_STOP(x) (1 << ((x)+15))

/*! @brief BSX activity bit identifiers */
typedef enum
{
    BSX_ID_ACTIVITY_BIT_NOCHANGE            = (0x00000000U),
    BSX_ID_ACTIVITY_BIT_STILL_START         = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_STILL),
    BSX_ID_ACTIVITY_BIT_WALK_START          = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_WALK),
    BSX_ID_ACTIVITY_BIT_RUN_START           = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_RUN),
    BSX_ID_ACTIVITY_BIT_BIKE_START          = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_BIKE),
    BSX_ID_ACTIVITY_BIT_INVEHICLE_START     = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_INVEHICLE),
    BSX_ID_ACTIVITY_BIT_TILTING_START       = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_TILTING),
    BSX_ID_ACTIVITY_BIT_VEHICLESTILL_START  = BSX_ID_ACTIVITY_START(BSX_ID_ACTIVITY_VEHICLESTILL),
    BSX_ID_ACTIVITY_BIT_STILL_STOP          = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_STILL),
    BSX_ID_ACTIVITY_BIT_WALK_STOP           = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_WALK),
    BSX_ID_ACTIVITY_BIT_RUN_STOP            = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_RUN),
    BSX_ID_ACTIVITY_BIT_BIKE_STOP           = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_BIKE),
    BSX_ID_ACTIVITY_BIT_INVEHICLE_STOP      = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_INVEHICLE),
    BSX_ID_ACTIVITY_BIT_TILTING_STOP        = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_TILTING),
    BSX_ID_ACTIVITY_BIT_VEHICLESTILL_STOP   = BSX_ID_ACTIVITY_STOP(BSX_ID_ACTIVITY_VEHICLESTILL)
} bsx_activity_bit_identifier_t;

#ifdef __cplusplus
}
#endif

#endif /*__BSX_ACTIVITY_BIT_IDENTIFIER_H__*/
/*! @}*/

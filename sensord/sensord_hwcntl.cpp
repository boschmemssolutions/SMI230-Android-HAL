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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <poll.h>
#include <fcntl.h>
#include <dirent.h>

#include "BoschSensor.h"
#include "bsx_android.h"
#include "bsx_datatypes.h"

#include "axis_remap.h"
#include "sensord_hwcntl.h"
#include "sensord_pltf.h"
#include "sensord_algo.h"
#include "util_misc.h"

/*
 * 1st byte major version of HAL
 * 2nd byte minor version of HAL
 * 3nd byte major version of driver which this HAL is based on
 * 4nd byte minor version of driver which this HAL is based on
 */
uint8_t HAL_ver[4] = { 0, 1, 0, 3 };

#define UNUSED_SENSOR_T(sensor_name) \
	{	.name = sensor_name,\
		.vendor = "Bosch",\
		.version = 1,\
		.handle = BSX_SENSOR_ID_INVALID,\
		.type = 0,\
		.maxRange = 1.0f,\
		.resolution = 1.0f,\
		.power = 0.0f,\
		.minDelay = 0,\
		.fifoReservedEventCount = 0,\
		.fifoMaxEventCount = 0,\
		.stringType = "",\
		.requiredPermission = NULL,\
		.maxDelay = 0,\
		.flags = SENSOR_FLAG_SPECIAL_REPORTING_MODE,\
		.reserved = { }\
	}

//map with enum BSX4_SENSORLIST_INX
struct sensor_t bosch_all_sensors[SENSORLIST_INX_END] = {
	{	.name = "BOSCH GAS Resist Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GAS_RESIST,
		.type = SENSOR_TYPE_BOSCH_GAS_RESIST,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_BOSCH_GAS_RESIST,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Accelerometer Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ACCELEROMETER,
		.type = SENSOR_TYPE_ACCELEROMETER,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 0.13f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ACCELEROMETER,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Magnetic Field Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_MAGNETIC_FIELD,
		.type = SENSOR_TYPE_MAGNETIC_FIELD,
		.maxRange = 1600.0f,
		.resolution = 0.3f,
		.power = 0.5f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_MAGNETIC_FIELD,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Orientation Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ORIENTATION,
		.type = SENSOR_TYPE_ORIENTATION,
		.maxRange = 360.0f,
		.resolution = 1.0f,
		.power = 0.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ORIENTATION,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Gyroscope Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GYROSCOPE,
		.type = SENSOR_TYPE_GYROSCOPE,
		.maxRange = 2500.0f,
		.resolution = 1.0f,
		.power = 5.0f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GYROSCOPE,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH light",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_LIGHT,
		.type = SENSOR_TYPE_LIGHT,
		.maxRange = 10000.0f,
		.resolution = 0.01f,
		.power = 0.175f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_LIGHT,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Pressure Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_PRESSURE,
		.type = SENSOR_TYPE_PRESSURE,
		.maxRange = 1100.0f,
		.resolution = 0.18f,
		.power = 4.0f,
		.minDelay = 160000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_PRESSURE,
		.requiredPermission = NULL,
		.maxDelay = 160000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Temperature Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_TEMPERATURE,
		.type = SENSOR_TYPE_TEMPERATURE,
		.maxRange = 200.0f,
		.resolution = 0.1f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_TEMPERATURE,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},

	UNUSED_SENSOR_T("BOSCH proximity")
, //nonwakeup proximity, not used

	{	.name = "BOSCH Gravity Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GRAVITY,
		.type = SENSOR_TYPE_GRAVITY,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GRAVITY,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Linear Acceleration Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_LINEAR_ACCELERATION,
		.type = SENSOR_TYPE_LINEAR_ACCELERATION,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_LINEAR_ACCELERATION,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Rotation Vector Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ROTATION_VECTOR,
		.type = SENSOR_TYPE_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Relative Humidity Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_RELATIVE_HUMIDITY,
		.type = SENSOR_TYPE_RELATIVE_HUMIDITY,
		.maxRange = 1.0f,
		.resolution = 0.01f,
		.power = 0.6f,
		.minDelay = 1000000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_RELATIVE_HUMIDITY,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Ambient Temperature Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_AMBIENT_TEMPERATURE,
		.type = SENSOR_TYPE_AMBIENT_TEMPERATURE,
		.maxRange = 200.0f,
		.resolution = 0.1f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_AMBIENT_TEMPERATURE,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Magnetic Field Uncalibrated Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_MAGNETIC_FIELD_UNCALIBRATED,
		.type = SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED,
		.maxRange = 1600.0f,
		.resolution = 0.3f,
		.power = 0.5f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_MAGNETIC_FIELD_UNCALIBRATED,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Game Rotation Vector Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GAME_ROTATION_VECTOR,
		.type = SENSOR_TYPE_GAME_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GAME_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Gyroscope Uncalibrated Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GYROSCOPE_UNCALIBRATED,
		.type = SENSOR_TYPE_GYROSCOPE_UNCALIBRATED,
		.maxRange = 2500.0f,
		.resolution = 1.0f,
		.power = 5.0f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GYROSCOPE_UNCALIBRATED,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},

	UNUSED_SENSOR_T("BOSCH significant motion nonwakeup")
, //Significant Motion, not used

	{	.name = "BOSCH Step Detector Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_STEP_DETECTOR,
		.type = SENSOR_TYPE_STEP_DETECTOR,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_SPECIAL,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_STEP_DETECTOR,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_SPECIAL,
		.flags = SENSOR_FLAG_SPECIAL_REPORTING_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Step Counter Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_STEP_COUNTER,
		.type = SENSOR_TYPE_STEP_COUNTER,
		.maxRange = 10000.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_STEP_COUNTER,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Geomagnetic Rotation Vector Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GEOMAGNETIC_ROTATION_VECTOR,
		.type = SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GEOMAGNETIC_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH heart rate",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_HEART_RATE,
		.type = SENSOR_TYPE_HEART_RATE,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_HEART_RATE,
		.requiredPermission = SENSOR_PERMISSION_BODY_SENSORS,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	UNUSED_SENSOR_T("BOSCH tilt detector nonwakeup")
, // not used
	UNUSED_SENSOR_T("BOSCH wake gesture nonwakeup")
, //Wake Gesture, not used
	UNUSED_SENSOR_T("BOSCH glance gesture nonwakeup")
, //Glance Gesture, not used
	UNUSED_SENSOR_T("BOSCH pickup gesture nonwakeup")
, //Pickup Gesture, not used
	UNUSED_SENSOR_T("BOSCH magnetic uncal offset")
, //magnetic field uncalibrated offset
	UNUSED_SENSOR_T("BOSCH gyroscope uncal offset")
, //gyroscope uncalibrated offset
	UNUSED_SENSOR_T("BOSCH power consumption nonwakeup")
, //power consumption, not used
	{	.name = "BOSCH Ambient Alcohol Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_AMBIENT_ALCOHOL,
		.type = SENSOR_TYPE_BOSCH_AMBIENT_ALCOHOL,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_BOSCH_AMBIENT_ALCOHOL,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Ambient CO2 Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_AMBIENT_CO2,
		.type = SENSOR_TYPE_BOSCH_AMBIENT_CO2,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_BOSCH_AMBIENT_CO2,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	{	.name = "BOSCH Ambient IAQ Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_AMBIENT_IAQ,
		.type = SENSOR_TYPE_BOSCH_AMBIENT_IAQ,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 0.1f,
		.minDelay = 1000000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_BOSCH_AMBIENT_IAQ,
		.requiredPermission = NULL,
		.maxDelay = 1000000,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE,
		.reserved = { }
	},
	UNUSED_SENSOR_T("BOSCH Signi Pressure (Wakeup) Sensor")
,
	{	.name = "BOSCH Accelerometer (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ACCELEROMETER_WAKEUP,
		.type = SENSOR_TYPE_ACCELEROMETER,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 0.13f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ACCELEROMETER,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Magnetic Field (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_MAGNETIC_FIELD_WAKEUP,
		.type = SENSOR_TYPE_MAGNETIC_FIELD,
		.maxRange = 1600.0f,
		.resolution = 0.3f,
		.power = 0.5f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_MAGNETIC_FIELD,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Orientation (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ORIENTATION_WAKEUP,
		.type = SENSOR_TYPE_ORIENTATION,
		.maxRange = 360.0f,
		.resolution = 1.0f,
		.power = 0.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ORIENTATION,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Gyroscope (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GYROSCOPE_WAKEUP,
		.type = SENSOR_TYPE_GYROSCOPE,
		.maxRange = 2500.0f,
		.resolution = 1.0f,
		.power = 5.0f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GYROSCOPE,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH light wakeup",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_LIGHT_WAKEUP,
		.type = SENSOR_TYPE_LIGHT,
		.maxRange = 10000.0f,
		.resolution = 0.01f,
		.power = 0.175f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_LIGHT,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Pressure (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_PRESSURE_WAKEUP,
		.type = SENSOR_TYPE_PRESSURE,
		.maxRange = 1100.0f,
		.resolution = 0.18f,
		.power = 4.0f,
		.minDelay = 10000,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_PRESSURE,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Temperature (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_TEMPERATURE_WAKEUP,
		.type = SENSOR_TYPE_TEMPERATURE,
		.maxRange = 200.0f,
		.resolution = 0.1f,
		.power = 0.1f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_TEMPERATURE,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH proximity wakeup",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_PROXIMITY_WAKEUP,
		.type = SENSOR_TYPE_PROXIMITY,
		.maxRange = 5.0f,
		.resolution = 0.1,
		.power = 12.675f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_PROXIMITY,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Gravity (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GRAVITY_WAKEUP,
		.type = SENSOR_TYPE_GRAVITY,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GRAVITY,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Linear Acceleration (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_LINEAR_ACCELERATION_WAKEUP,
		.type = SENSOR_TYPE_LINEAR_ACCELERATION,
		.maxRange = (2.0f * GRAVITY_EARTH),
		.resolution = 0.1f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_LINEAR_ACCELERATION,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Rotation Vector (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ROTATION_VECTOR_WAKEUP,
		.type = SENSOR_TYPE_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Relative Humidity (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_RELATIVE_HUMIDITY_WAKEUP,
		.type = SENSOR_TYPE_RELATIVE_HUMIDITY,
		.maxRange = 1.0f,
		.resolution = 0.01f,
		.power = 0.6f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_RELATIVE_HUMIDITY,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Ambient Temperature (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_AMBIENT_TEMPERATURE_WAKEUP,
		.type = SENSOR_TYPE_AMBIENT_TEMPERATURE,
		.maxRange = 200.0f,
		.resolution = 0.1f,
		.power = 0.1f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_AMBIENT_TEMPERATURE,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Magnetic Uncalibrated (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_MAGNETIC_FIELD_UNCALIBRATED_WAKEUP,
		.type = SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED,
		.maxRange = 1600.0f,
		.resolution = 0.3f,
		.power = 0.5f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_MAGNETIC_FIELD_UNCALIBRATED,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Game Rotation Vector (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GAME_ROTATION_VECTOR_WAKEUP,
		.type = SENSOR_TYPE_GAME_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GAME_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Gyroscope Uncalibrated (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GYROSCOPE_UNCALIBRATED_WAKEUP,
		.type = SENSOR_TYPE_GYROSCOPE_UNCALIBRATED,
		.maxRange = 2500.0f,
		.resolution = 1.0f,
		.power = 5.0f,
		.minDelay = BST_SENSOR_MINDELAY_uS,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GYROSCOPE_UNCALIBRATED,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Significant Motion Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_SIGNIFICANT_MOTION_WAKEUP,
		.type = SENSOR_TYPE_SIGNIFICANT_MOTION,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.13f,
		.minDelay = SENSOR_MINDELAY_ONESHOT,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_SIGNIFICANT_MOTION,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONESHOT,
		.flags = SENSOR_FLAG_ONE_SHOT_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Step Detector (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_STEP_DETECTOR_WAKEUP,
		.type = SENSOR_TYPE_STEP_DETECTOR,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_SPECIAL,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_STEP_DETECTOR,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_SPECIAL,
		.flags = SENSOR_FLAG_SPECIAL_REPORTING_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Step Counter (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_STEP_COUNTER_WAKEUP,
		.type = SENSOR_TYPE_STEP_COUNTER,
		.maxRange = 10000.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_STEP_COUNTER,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Geomagnetic Rotation Vector (Wakeup) Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GEOMAGNETIC_ROTATION_VECTOR_WAKEUP,
		.type = SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR,
		.maxRange = 1.0f,
		.resolution = 0.00003f,
		.power = 5.63f,
		.minDelay = 40000,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_GEOMAGNETIC_ROTATION_VECTOR,
		.requiredPermission = NULL,
		.maxDelay = 200000,
		.flags = SENSOR_FLAG_CONTINUOUS_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH heart rate wakeup",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_HEART_RATE_WAKEUP,
		.type = SENSOR_TYPE_HEART_RATE,
		.maxRange = 1000.0f,
		.resolution = 1.0f,
		.power = 5.63f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = BATCH_RSV_FRAME_COUNT,
		.fifoMaxEventCount = BATCH_MAX_FRAME_COUNT,
		.stringType = SENSOR_STRING_TYPE_HEART_RATE,
		.requiredPermission = SENSOR_PERMISSION_BODY_SENSORS,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Tilt Detector Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_TILT_DETECTOR_WAKEUP,
		.type = SENSOR_TYPE_TILT_DETECTOR,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.0f,
		.minDelay = SENSOR_MINDELAY_SPECIAL,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_TILT_DETECTOR,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_SPECIAL,
		.flags = SENSOR_FLAG_SPECIAL_REPORTING_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Wake Gesture Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_WAKE_GESTURE_WAKEUP,
		.type = SENSOR_TYPE_WAKE_GESTURE,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.0f,
		.minDelay = SENSOR_MINDELAY_ONESHOT,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_WAKE_GESTURE,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONESHOT,
		.flags = SENSOR_FLAG_ONE_SHOT_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Glance Gesture Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_GLANCE_GESTURE_WAKEUP,
		.type = SENSOR_TYPE_GLANCE_GESTURE,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.0f,
		.minDelay = SENSOR_MINDELAY_ONESHOT,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_GLANCE_GESTURE,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONESHOT,
		.flags = SENSOR_FLAG_ONE_SHOT_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},
	{	.name = "BOSCH Pickup Gesture Sensor",
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_PICK_UP_GESTURE_WAKEUP,
		.type = SENSOR_TYPE_PICK_UP_GESTURE,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.0f,
		.minDelay = SENSOR_MINDELAY_ONESHOT,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_PICK_UP_GESTURE,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONESHOT,
		.flags = SENSOR_FLAG_ONE_SHOT_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},

	UNUSED_SENSOR_T("BOSCH magnetic uncal offset wakeup")
, //magnetic field uncalibrated offset wakeup
	UNUSED_SENSOR_T("BOSCH gyroscope uncal offset wakeup")
, //gyroscope uncalibrated offset wakeup
	UNUSED_SENSOR_T("BOSCH power consumption wakeup")
, //Power Consumption Wakeup, not used

	{	.name = "BOSCH Activity Recognition Sensor", /** to test AR in sensor HAL, name is pre-defined by test app */
		.vendor = "Bosch",
		.version = 1,
		.handle = BSX_SENSOR_ID_ACTIVITY,
		.type = SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION,
		.maxRange = 1.0f,
		.resolution = 1.0f,
		.power = 0.0f,
		.minDelay = SENSOR_MINDELAY_ONCHANGE,
		.fifoReservedEventCount = 0,
		.fifoMaxEventCount = 0,
		.stringType = SENSOR_STRING_TYPE_BOSCH_AR,
		.requiredPermission = NULL,
		.maxDelay = SENSOR_MAXDELAY_ONCHANGE,
		.flags = SENSOR_FLAG_ON_CHANGE_MODE | SENSOR_FLAG_WAKE_UP,
		.reserved = { }
	},

    UNUSED_SENSOR_T("BOSCH Ambient CO2 (Wakeup) Sensor")
,
	UNUSED_SENSOR_T("BOSCH Ambient IAQ (Wakeup) Sensor")
,
};

BST_SENSORLIST bosch_sensorlist = { NULL, NULL, 0 };
uint32_t active_nonwksensor_cnt = 0;
uint32_t active_wksensor_cnt = 0;

void get_sensor_t(int32_t sensor_id, struct sensor_t **pp_sensor_t, int32_t *p_list_inx)
{
    uint32_t i;
    struct sensor_t *p_sensor = NULL;

    for (i = 0; i < bosch_sensorlist.list_len; i++)
    {
        if (NULL == bosch_sensorlist.list[i].name)
        {
            //NULL == name means sensor is not supported so that structure is unuseful
            continue;
        }
        else
        {
            if (sensor_id == bosch_sensorlist.list[i].handle)
            {
                p_sensor = &(bosch_sensorlist.list[i]);
                break;
            }
        }
    }

    if (i != bosch_sensorlist.list_len)
    {
        *pp_sensor_t = p_sensor;
        *p_list_inx = bosch_sensorlist.bsx_list_index[i];
    }
    else
    {
        /*not found*/
        *pp_sensor_t = NULL;
    }

    return;
}

/*default: disabled, data rate = 1Hz, unit = second, max dalay = 10s*/
#define DEFAULT_SENSOR_CONFIG(odr, unit, latency, len, delay) \
	{	.data_rate = odr,\
		.latency_unit = unit,\
		.max_latency = latency,\
		.fifo_data_len = len,\
		.delay_onchange_Hz = delay,\
	}

//map with enum BSX4_SENSORLIST_INX
BSX_SENSOR_CONFIG BSX_sensor_config_nonwk[SENSORLIST_INX_AMBIENT_IAQ+1] = {

	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 4, 0)
,    //SENSORLIST_INX_GAS_RESIST
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_ACCELEROMETER
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_MAGNETIC_FIELD
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_ORIENTATION,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_GYROSCOPE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_LIGHT,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_6_25Hz, BSX_CONFSTR_UNITms, 200, 3, 0)
,    //SENSORLIST_INX_PRESSURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_TEMPERATURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 2, 0)
,    //SENSORLIST_INX_PROXIMITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_GRAVITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_LINEAR_ACCELERATION,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 9, 0)
,    //SENSORLIST_INX_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_RELATIVE_HUMIDITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_AMBIENT_TEMPERATURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 6, 0)
,    //SENSORLIST_INX_MAGNETIC_FIELD_UNCALIBRATED,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 8, 0)
,    //SENSORLIST_INX_GAME_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 6, 0)
,    //SENSORLIST_INX_GYROSCOPE_UNCALIBRATED,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_SIGNIFICANT_MOTION,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_STEP_DETECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 4, 5)
,    //SENSORLIST_INX_STEP_COUNTER,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 9, 0)
,    //SENSORLIST_INX_MAGNETIC_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 1, 5)
,    //SENSORLIST_INX_HEART_RATE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_TILT_DETECTOR,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKE_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_GLANCE_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_PICK_UP_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 6, 0)
,    //SENSORLIST_INX_MAGNETIC_FIELD_UNCALIBRATED_OFFSET
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 6, 0)
,    //SENSORLIST_INX_GYROSCOPE_UNCALIBRATED_OFFSET,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 3, 0)
,    //SENSORLIST_INX_POWER_CONSUMPTION,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 0, 2, 5)
,    //SENSORLIST_INX_AMBIENT_ALCOHOL,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 0, 2, 5)
,    //SENSORLIST_INX_AMBIENT_CO2,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 0, 2, 5)
, //SENSORLIST_INX_AMBIENT_IAQ,
};

/*more one NULL pointer for end*/
BSX_SENSOR_CONFIG * BSX_active_confref_nonwk[SENSORLIST_INX_AMBIENT_IAQ + 1] = { NULL };

//map with enum BSX4_SENSORLIST_INX
//include a TIMESTAMP
BSX_SENSOR_CONFIG BSX_sensor_config_wk[SENSORLIST_INX_END-SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE] = {

	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_ACCELEROMETER,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_ORIENTATION,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_GYROSCOPE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_WAKEUP_LIGHT,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 3, 0)
,    //SENSORLIST_INX_WAKEUP_PRESSURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_WAKEUP_TEMPERATURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_WAKEUP_PROXIMITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_GRAVITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 7, 0)
,    //SENSORLIST_INX_WAKEUP_LINEAR_ACCELERATION,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 9, 0)
,    //SENSORLIST_INX_WAKEUP_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_WAKEUP_RELATIVE_HUMIDITY,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 200, 2, 5)
,    //SENSORLIST_INX_WAKEUP_AMBIENT_TEMPERATURE,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 6, 0)
,    //SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 8, 0)
,    //SENSORLIST_INX_WAKEUP_GAME_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 6, 0)
,    //SENSORLIST_INX_WAKEUP_GYROSCOPE_UNCALIBRATED,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_SIGNIFICANT_MOTION,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_STEP_DETECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 4, 5)
,    //SENSORLIST_INX_WAKEUP_STEP_COUNTER,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 9, 0)
,    //SENSORLIST_INX_WAKEUP_MAGNETIC_ROTATION_VECTOR,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_50Hz, BSX_CONFSTR_UNITms, 200, 1, 5)
,    //SENSORLIST_INX_WAKEUP_HEART_RATE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_TILT_DETECTOR,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_WAKE_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_GLANCE_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_PICK_UP_GESTURE,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 6, 0)
,    //SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED_OFFSET
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 6, 0)
,    //SENSORLIST_INX_WAKEUP_GYROSCOPE_UNCALIBRATED_OFFSET
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 0, 0)
,    //SENSORLIST_INX_WAKEUP_POWER_CONSUMPTION,
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 2, 5)
,    //SENSORLIST_INX_WAKEUP_ACTIVITY
	DEFAULT_SENSOR_CONFIG( 0, 0, 0, 2, 0)
,    //SENSORLIST_INX_WAKEUP_AMBIENT_CO2,
	DEFAULT_SENSOR_CONFIG( BSX_CONFSTR_1Hz, BSX_CONFSTR_UNITms, 0, 2, 0)
, //SENSORLIST_INX_WAKEUP_AMBIENT_IAQ,
};

/*more one NULL pointer for end*/
BSX_SENSOR_CONFIG * BSX_active_confref_wk[SENSORLIST_INX_END - SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE] = { NULL };

static uint8_t encode_datarate(int64_t sampling_period_ns)
{
    float Hz;

    if (0 == sampling_period_ns)
    {
        // handle on change/on shot/special type sensors
        // and should avoid divide 0 exception
        return 0;
    }

    Hz = 1000000000.0f / (float)sampling_period_ns;

    if (Hz > 200)
    {
        return BSX_CONFSTR_400Hz;
    }
    if (Hz > 100 && Hz <= 200)
    {
        return BSX_CONFSTR_200Hz;
    }
    if (Hz > 50 && Hz <= 100)
    {
        return BSX_CONFSTR_100Hz;
    }
    if (Hz > 25 && Hz <= 50)
    {
        return BSX_CONFSTR_50Hz;
    }
    if (Hz > 12 && Hz <= 25)
    {
        return BSX_CONFSTR_25Hz;
    }
    if (Hz > 6 && Hz <= 12)
    {
        return BSX_CONFSTR_12_5Hz;
    }
    if (Hz > 1 && Hz <= 6)
    {
        return BSX_CONFSTR_6_25Hz;
    }
    if (Hz > 0.5 && Hz <= 1)
    {
        return BSX_CONFSTR_1Hz;
    }
    if (Hz > 0.25 && Hz <= 0.5)
    {
        return BSX_CONFSTR_0_5Hz;
    }
    if (Hz > 0.125 && Hz <= 0.25)
    {
        return BSX_CONFSTR_0_25Hz;
    }
    if (Hz > 0.0625 && Hz <= 0.125)
    {
        return BSX_CONFSTR_0_125Hz;
    }
    if (Hz > 0.03125 && Hz <= 0.0625)
    {
        return BSX_CONFSTR_0_0625Hz;
    }
    if (Hz > 0.015625 && Hz <= 0.03125)
    {
        return BSX_CONFSTR_0_03125Hz;
    }
    if (Hz <= 0.015625)
    {
        return BSX_CONFSTR_0_015625Hz;
    }

    return 0;
}

static void encode_max_latency(int64_t max_report_latency_ns, uint16_t *value, uint8_t *unit)
{
    /*optimize: most used config is arranged before*/
    if (max_report_latency_ns >= 1000000LL && max_report_latency_ns < 1000000000LL)
    {
        *value = (uint16_t)(max_report_latency_ns / 1000000LL);
        *unit = BSX_CONFSTR_UNITms;
    }
    else
    if (max_report_latency_ns >= 1000LL && max_report_latency_ns < 1000000LL)
    {
        *value = (uint16_t)(max_report_latency_ns / 1000LL);
        *unit = BSX_CONFSTR_UNITus;
    }
    else
    if (max_report_latency_ns < 1000LL)
    {
        *value = (uint16_t) max_report_latency_ns;
        *unit = BSX_CONFSTR_UNITns;
    }
    else
        if (max_report_latency_ns >= 1000000000LL && max_report_latency_ns < 16383000000000LL)
        {
            *value = (uint16_t)(max_report_latency_ns / 1000000000LL);
            *unit = BSX_CONFSTR_UNITs;
        }
        else
        {
            *value = 16383;
            *unit = BSX_CONFSTR_UNITs;
        }

    return;
}

/**
 * @return: indicate if config string need to send
 */
int activate_configref_resort(int32_t bsx_list_index, int32_t is_enable)
{
    int i;
    int32_t bsx_listinx_base;
    int loop_end;
    int32_t current_bsxindex;
    BSX_SENSOR_CONFIG *p_config;
    BSX_SENSOR_CONFIG **p_config_refers;
    uint32_t *p_active_sensor_cnt;

    if (bsx_list_index <= SENSORLIST_INX_AMBIENT_IAQ)
    {
        bsx_listinx_base = SENSORLIST_INX_GAS_RESIST;
        loop_end = SENSORLIST_INX_AMBIENT_IAQ - SENSORLIST_INX_GAS_RESIST + 1;
        p_config = BSX_sensor_config_nonwk;
        p_config_refers = BSX_active_confref_nonwk;
        p_active_sensor_cnt = &active_nonwksensor_cnt;
    }
    else
    {
        bsx_listinx_base = SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE;
        loop_end = SENSORLIST_INX_END - SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE;
        p_config = BSX_sensor_config_wk;
        p_config_refers = BSX_active_confref_wk;
        p_active_sensor_cnt = &active_wksensor_cnt;
    }

    for (i = 0; i < loop_end; i++)
    {
        if (NULL == p_config_refers[i])
        {
            if (is_enable)
            {
                /*find a blank and insert the current */
                p_config_refers[i] = &p_config[bsx_list_index - bsx_listinx_base];
                (*p_active_sensor_cnt)++;

                return 1;
            }else
            {
                /*nothing to delete*/
                return 0;
            }
        }

        current_bsxindex = bsx_listinx_base + (int32_t)(p_config_refers[i] - p_config);
        if (bsx_list_index > current_bsxindex)
        {
            continue;
        }
        else if (bsx_list_index == current_bsxindex)
        {
            if (is_enable)
            {
                /*duplicated enable, ignore*/
                return 0;
            }else
            {
                (*p_active_sensor_cnt)--;

                memmove(&p_config_refers[i], &p_config_refers[i+1],
                        ((int)(*p_active_sensor_cnt) - i) * sizeof(p_config_refers[0]));
                p_config_refers[*p_active_sensor_cnt] = NULL;

                return 1;
            }
        }else if (bsx_list_index < current_bsxindex)
        {
            if (is_enable)
            {
                memmove(&p_config_refers[i+1], &p_config_refers[i],
                        ((int)(*p_active_sensor_cnt) - i) * sizeof(p_config_refers[0]));

                /*insert the current */
                p_config_refers[i] = &p_config[bsx_list_index - bsx_listinx_base];
                (*p_active_sensor_cnt)++;

                return 1;
            }else
            {
                /*can't found the target reference, ignore*/
                return 0;
            }

        }
    }

    return 0;

}

/**
 *
 * @return: indicate if config string need to send
 */
int batch_configref_resort(int32_t bsx_list_index, int64_t sampling_period_ns, int64_t max_report_latency_ns, float delay_Hz)
{
    uint32_t i;
    int32_t bsx_listinx_base;
    int32_t current_bsxindex;
    BSX_SENSOR_CONFIG *p_config;
    BSX_SENSOR_CONFIG **p_config_refers;
    uint32_t *p_active_sensor_cnt;

    if (bsx_list_index <= SENSORLIST_INX_AMBIENT_IAQ)
    {
        bsx_listinx_base = SENSORLIST_INX_GAS_RESIST;
        p_config = BSX_sensor_config_nonwk;
        p_config_refers = BSX_active_confref_nonwk;
        p_active_sensor_cnt = &active_nonwksensor_cnt;
    }
    else
    {
        bsx_listinx_base = SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE;
        p_config = BSX_sensor_config_wk;
        p_config_refers = BSX_active_confref_wk;
        p_active_sensor_cnt = &active_wksensor_cnt;
    }

    for (i = 0; i < (*p_active_sensor_cnt); i++)
    {
        current_bsxindex = bsx_listinx_base + (int32_t)(p_config_refers[i] - p_config);

        if (bsx_list_index == current_bsxindex)
        {
            p_config_refers[i]->data_rate = encode_datarate(sampling_period_ns);
            encode_max_latency(max_report_latency_ns,
                    &(p_config_refers[i]->max_latency),
                    &(p_config_refers[i]->latency_unit));
            p_config_refers[i]->delay_onchange_Hz = delay_Hz;
            return 1;
        }
    }

    p_config[bsx_list_index - bsx_listinx_base].data_rate = encode_datarate(sampling_period_ns);
    encode_max_latency(max_report_latency_ns,
            &(p_config[bsx_list_index - bsx_listinx_base].max_latency),
            &(p_config[bsx_list_index - bsx_listinx_base].latency_unit));
    p_config[bsx_list_index - bsx_listinx_base].delay_onchange_Hz = delay_Hz;

    return 0;

}


typedef struct
{
    int32_t bsx_list_inx;
    int32_t supl_id;
} BSX_INX_SPLID_MAPTBL;

static const BSX_INX_SPLID_MAPTBL bsx_inx_splid_maptbl[] = {
        { SENSORLIST_INX_ACCELEROMETER, BSX_OUTPUT_ID_ACCELERATION_CORRECTED },
        { SENSORLIST_INX_MAGNETIC_FIELD, BSX_OUTPUT_ID_MAGNETICFIELD_CORRECTED },
        { SENSORLIST_INX_ORIENTATION, BSX_OUTPUT_ID_ORIENTATION },
        { SENSORLIST_INX_GYROSCOPE, BSX_OUTPUT_ID_ANGULARRATE_CORRECTED },
        { SENSORLIST_INX_GRAVITY, BSX_OUTPUT_ID_GRAVITY },
        { SENSORLIST_INX_LINEAR_ACCELERATION, BSX_OUTPUT_ID_LINEARACCELERATION },
        { SENSORLIST_INX_ROTATION_VECTOR, BSX_OUTPUT_ID_ROTATION },
        { SENSORLIST_INX_MAGNETIC_FIELD_UNCALIBRATED, BSX_OUTPUT_ID_MAGNETICFIELD_RAW },
        { SENSORLIST_INX_GAME_ROTATION_VECTOR, BSX_OUTPUT_ID_ROTATION_GAME },
        { SENSORLIST_INX_GYROSCOPE_UNCALIBRATED, BSX_OUTPUT_ID_ANGULARRATE_RAW },
        { SENSORLIST_INX_STEP_DETECTOR, BSX_OUTPUT_ID_STEPDETECTOR },
        { SENSORLIST_INX_STEP_COUNTER, BSX_OUTPUT_ID_STEPCOUNTER },
        { SENSORLIST_INX_MAGNETIC_ROTATION_VECTOR, BSX_OUTPUT_ID_ROTATION_GEOMAGNETIC },
        { SENSORLIST_INX_WAKEUP_ACCELEROMETER, BSX_WAKEUP_ID_ACCELERATION_CORRECTED },
        { SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD, BSX_WAKEUP_ID_MAGNETICFIELD_CORRECTED },
        { SENSORLIST_INX_WAKEUP_ORIENTATION, BSX_WAKEUP_ID_ORIENTATION },
        { SENSORLIST_INX_WAKEUP_GYROSCOPE, BSX_WAKEUP_ID_ANGULARRATE_CORRECTED },
        { SENSORLIST_INX_WAKEUP_GRAVITY, BSX_WAKEUP_ID_GRAVITY },
        { SENSORLIST_INX_WAKEUP_LINEAR_ACCELERATION, BSX_WAKEUP_ID_LINEARACCELERATION },
        { SENSORLIST_INX_WAKEUP_ROTATION_VECTOR, BSX_WAKEUP_ID_ROTATION },
        { SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED, BSX_WAKEUP_ID_MAGNETICFIELD_RAW },
        { SENSORLIST_INX_WAKEUP_GAME_ROTATION_VECTOR, BSX_WAKEUP_ID_ROTATION_GAME },
        { SENSORLIST_INX_WAKEUP_GYROSCOPE_UNCALIBRATED, BSX_WAKEUP_ID_ANGULARRATE_RAW },
        { SENSORLIST_INX_WAKEUP_SIGNIFICANT_MOTION, BSX_OUTPUT_ID_SIGNIFICANTMOTION_STATUS },
        /** SENSORLIST_INX_WAKEUP_STEP_DETECTOR: No wakeup step detector in library's output gates*/
        { SENSORLIST_INX_WAKEUP_STEP_COUNTER, BSX_WAKEUP_ID_STEPCOUNTER },
        { SENSORLIST_INX_WAKEUP_MAGNETIC_ROTATION_VECTOR, BSX_WAKEUP_ID_ROTATION_GEOMAGNETIC },
        { SENSORLIST_INX_WAKEUP_TILT_DETECTOR, BSX_OUTPUT_ID_TILT_STATUS },
        { SENSORLIST_INX_WAKEUP_WAKE_GESTURE, BSX_OUTPUT_ID_WAKE_STATUS },
        { SENSORLIST_INX_WAKEUP_GLANCE_GESTURE, BSX_OUTPUT_ID_GLANCE_STATUS },
        { SENSORLIST_INX_WAKEUP_PICK_UP_GESTURE, BSX_OUTPUT_ID_PICKUP_STATUS },
        { SENSORLIST_INX_WAKEUP_ACTIVITY, BSX_OUTPUT_ID_ACTIVITY },
};

int32_t convert_BSX_ListInx(int32_t bsx_list_inx)
{
    int32_t loop;
    int32_t i;

    loop = ARRAY_ELEMENTS(bsx_inx_splid_maptbl);

    for (i = 0; i < loop; i++)
    {
        if (bsx_inx_splid_maptbl[i].bsx_list_inx == bsx_list_inx)
        {
            return bsx_inx_splid_maptbl[i].supl_id;
        }
    }

    return BSX_VIRTUAL_SENSOR_ID_INVALID;
}


/**
 *
 * @param event_name: target event's name
 * @param p_fd: when found, open the event node
 * @param p_num: when found, get the number of event node
 */
void open_input_by_name(const char *event_name, int *p_fd, int *p_num)
{
    int fd = -1;
    const char *dirname = "/dev/input";
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    int ret;
    struct dirent entry;
    struct dirent *result;

    dir = opendir(dirname);
    if (dir == NULL)
    {
        PERR("couldn't open dir '%s'", dirname);
        return;
    }

    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';

    while (1)
    {
        ret = readdir_r(dir, &entry, &result);
        if (0 != ret || NULL == result)
        {
            //error or end of directory stream
            break;
        }

        if (entry.d_name[0] == '.' &&
                (entry.d_name[1] == '\0' ||
                        (entry.d_name[1] == '.' && entry.d_name[2] == '\0')))
        {
            continue;
        }

        strcpy(filename, entry.d_name);
        fd = open(devname, O_RDONLY | O_NONBLOCK);
        if (fd >= 0)
        {
            char name[80];
            if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1)
            {
                name[0] = '\0';
            }
            if (!strcmp(name, event_name))
            {
                sscanf(entry.d_name + strlen("event"), "%8d", p_num);
                break;
            }
            else
            {
                close(fd);
                fd = -1;
            }
        }
    }

    closedir(dir);

    if (fd < 0)
    {
        PERR("couldn't find '%s' input device", event_name);
    }

    *p_fd = fd;

    return;
}




void *hwcntl_main(void *arg)
{
    int ret;

    BoschSensor *bosch_sensor = reinterpret_cast<BoschSensor *>(arg);

    if(bosch_sensor->pfun_hw_deliver_sensordata)
    {
        while (1)
        {
            bosch_sensor->pfun_hw_deliver_sensordata(bosch_sensor);
        }
    }

     //should not run here
    pthread_exit((void *) &ret);

    return NULL;
}


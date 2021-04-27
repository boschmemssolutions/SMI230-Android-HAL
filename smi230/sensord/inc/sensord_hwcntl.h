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

#ifndef __SENSORD_HWCNTL_H
#define __SENSORD_HWCNTL_H

enum BSX4_SENSORLIST_INX
{
    SENSORLIST_INX_GAS_RESIST = 0,
    SENSORLIST_INX_ACCELEROMETER = 1,
    SENSORLIST_INX_MAGNETIC_FIELD = 2,
    SENSORLIST_INX_ORIENTATION = 3,
    SENSORLIST_INX_GYROSCOPE = 4,
    SENSORLIST_INX_LIGHT = 5,
    SENSORLIST_INX_PRESSURE = 6,
    SENSORLIST_INX_TEMPERATURE = 7,
    SENSORLIST_INX_PROXIMITY = 8,
    SENSORLIST_INX_GRAVITY = 9,
    SENSORLIST_INX_LINEAR_ACCELERATION = 10,
    SENSORLIST_INX_ROTATION_VECTOR = 11,
    SENSORLIST_INX_RELATIVE_HUMIDITY = 12,
    SENSORLIST_INX_AMBIENT_TEMPERATURE = 13,
    SENSORLIST_INX_MAGNETIC_FIELD_UNCALIBRATED = 14,
    SENSORLIST_INX_GAME_ROTATION_VECTOR = 15,
    SENSORLIST_INX_GYROSCOPE_UNCALIBRATED = 16,
    SENSORLIST_INX_SIGNIFICANT_MOTION = 17,
    SENSORLIST_INX_STEP_DETECTOR = 18,
    SENSORLIST_INX_STEP_COUNTER = 19,
    SENSORLIST_INX_MAGNETIC_ROTATION_VECTOR = 20,
    SENSORLIST_INX_HEART_RATE = 21,
    SENSORLIST_INX_TILT_DETECTOR = 22,
    SENSORLIST_INX_WAKE_GESTURE = 23,
    SENSORLIST_INX_GLANCE_GESTURE = 24,
    SENSORLIST_INX_PICK_UP_GESTURE = 25,
    SENSORLIST_INX_MAGNETIC_FIELD_UNCALIBRATED_OFFSET = 26,
    SENSORLIST_INX_GYROSCOPE_UNCALIBRATED_OFFSET = 27,
    SENSORLIST_INX_POWER_CONSUMPTION = 28,
    SENSORLIST_INX_AMBIENT_ALCOHOL = 29,
    SENSORLIST_INX_AMBIENT_CO2 = 30,
    SENSORLIST_INX_AMBIENT_IAQ = 31,

    SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE = 32,
    SENSORLIST_INX_WAKEUP_ACCELEROMETER = 33,
    SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD = 34,
    SENSORLIST_INX_WAKEUP_ORIENTATION = 35,
    SENSORLIST_INX_WAKEUP_GYROSCOPE = 36,
    SENSORLIST_INX_WAKEUP_LIGHT = 37,
    SENSORLIST_INX_WAKEUP_PRESSURE = 38,
    SENSORLIST_INX_WAKEUP_TEMPERATURE = 39,
    SENSORLIST_INX_WAKEUP_PROXIMITY = 40,
    SENSORLIST_INX_WAKEUP_GRAVITY = 41,
    SENSORLIST_INX_WAKEUP_LINEAR_ACCELERATION = 42,
    SENSORLIST_INX_WAKEUP_ROTATION_VECTOR = 43,
    SENSORLIST_INX_WAKEUP_RELATIVE_HUMIDITY = 44,
    SENSORLIST_INX_WAKEUP_AMBIENT_TEMPERATURE = 45,
    SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED = 46,
    SENSORLIST_INX_WAKEUP_GAME_ROTATION_VECTOR = 47,
    SENSORLIST_INX_WAKEUP_GYROSCOPE_UNCALIBRATED = 48,
    SENSORLIST_INX_WAKEUP_SIGNIFICANT_MOTION = 49,
    SENSORLIST_INX_WAKEUP_STEP_DETECTOR = 50,
    SENSORLIST_INX_WAKEUP_STEP_COUNTER = 51,
    SENSORLIST_INX_WAKEUP_MAGNETIC_ROTATION_VECTOR = 52,
    SENSORLIST_INX_WAKEUP_HEART_RATE = 53,
    SENSORLIST_INX_WAKEUP_TILT_DETECTOR = 54,
    SENSORLIST_INX_WAKEUP_WAKE_GESTURE = 55,
    SENSORLIST_INX_WAKEUP_GLANCE_GESTURE = 56,
    SENSORLIST_INX_WAKEUP_PICK_UP_GESTURE = 57,
    SENSORLIST_INX_WAKEUP_MAGNETIC_FIELD_UNCALIBRATED_OFFSET = 58,
    SENSORLIST_INX_WAKEUP_GYROSCOPE_UNCALIBRATED_OFFSET = 59,
    SENSORLIST_INX_WAKEUP_POWER_CONSUMPTION = 60,
    SENSORLIST_INX_WAKEUP_ACTIVITY = 61,
    SENSORLIST_INX_WAKEUP_AMBIENT_CO2 = 62,
    SENSORLIST_INX_WAKEUP_AMBIENT_IAQ = 63,
    SENSORLIST_INX_END,
};

/*!bosch sensor generic power mode enum */
enum BST_DEV_OP_MODE
{
    SENSOR_PM_NORMAL = 0,
    SENSOR_PM_LP1,
    SENSOR_PM_SUSPEND,
    SENSOR_PM_LP2
};

#define SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION              (SENSOR_TYPE_DEVICE_PRIVATE_BASE + 31)
#define SENSOR_STRING_TYPE_BOSCH_AR "com.bosch-BoschSensor.www.activityrecognition"

#define BSX_SENSOR_ID_AMBIENT_ALCOHOL               100
#define SENSOR_TYPE_BOSCH_AMBIENT_ALCOHOL           (SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION + 1)
#define SENSOR_STRING_TYPE_BOSCH_AMBIENT_ALCOHOL    "com.bosch-BoschSensor.www.ALCOHOL"
#define BSX_SENSOR_ID_AMBIENT_CO2                   101
#define SENSOR_TYPE_BOSCH_AMBIENT_CO2               (SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION + 2)
#define SENSOR_STRING_TYPE_BOSCH_AMBIENT_CO2        "com.bosch-BoschSensor.www.CO2"
#define BSX_SENSOR_ID_AMBIENT_IAQ                   102
#define SENSOR_TYPE_BOSCH_AMBIENT_IAQ               (SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION + 3)
#define SENSOR_STRING_TYPE_BOSCH_AMBIENT_IAQ        "com.bosch-BoschSensor.www.IAQ"
#define BSX_SENSOR_ID_GAS_RESIST                    103
#define SENSOR_TYPE_BOSCH_GAS_RESIST                (SENSOR_TYPE_BOSCH_ACTIVITY_RECOGNITION + 4)
#define SENSOR_STRING_TYPE_BOSCH_GAS_RESIST         "com.bosch-BoschSensor.www.GAS"

#define BSX_CONFSTR_400Hz  1
#define BSX_CONFSTR_200Hz  2
#define BSX_CONFSTR_100Hz  3
#define BSX_CONFSTR_50Hz   4
#define BSX_CONFSTR_25Hz   5
#define BSX_CONFSTR_12_5Hz 6
#define BSX_CONFSTR_6_25Hz 7
#define BSX_CONFSTR_1Hz    8
#define BSX_CONFSTR_0_5Hz    9
#define BSX_CONFSTR_0_25Hz    10
#define BSX_CONFSTR_0_125Hz    11
#define BSX_CONFSTR_0_0625Hz    12
#define BSX_CONFSTR_0_03125Hz    13
#define BSX_CONFSTR_0_015625Hz    14

#define BSX_CONFSTR_UNITns 0x00  //00000000
#define BSX_CONFSTR_UNITus 0x40  //01000000
#define BSX_CONFSTR_UNITms 0x80  //10000000
#define BSX_CONFSTR_UNITs  0xC0  //11000000


#define CONVERT_DATARATE_CODE(code, rate) \
    switch (code)\
    {\
        case BSX_CONFSTR_400Hz:\
            rate = 400;\
            break;\
        case BSX_CONFSTR_200Hz:\
            rate = 200;\
            break;\
        case BSX_CONFSTR_100Hz:\
            rate = 100;\
            break;\
        case BSX_CONFSTR_50Hz:\
            rate = 50;\
            break;\
        case BSX_CONFSTR_25Hz:\
            rate = 25;\
            break;\
        case BSX_CONFSTR_12_5Hz:\
            rate = 12.5;\
            break;\
        case BSX_CONFSTR_6_25Hz:\
            rate = 6.25;\
            break;\
        case BSX_CONFSTR_1Hz:\
            rate = 1;\
            break;\
        default:\
            rate = 0;\
    }\


typedef struct
{
    uint8_t data_rate;
    uint8_t latency_unit;
    uint16_t max_latency;
    uint16_t fifo_data_len;
    float delay_onchange_Hz;
} BSX_SENSOR_CONFIG;

typedef struct
{
    struct sensor_t *list;
    int32_t *bsx_list_index;
    uint32_t list_len;
} BST_SENSORLIST;

extern uint8_t HAL_ver[];
extern struct sensor_t bosch_all_sensors[SENSORLIST_INX_END];
extern BST_SENSORLIST bosch_sensorlist;

extern BSX_SENSOR_CONFIG BSX_sensor_config_nonwk[SENSORLIST_INX_AMBIENT_IAQ + 1];
extern BSX_SENSOR_CONFIG * BSX_active_confref_nonwk[SENSORLIST_INX_AMBIENT_IAQ + 1];
extern BSX_SENSOR_CONFIG BSX_sensor_config_wk[SENSORLIST_INX_END - SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE];
extern BSX_SENSOR_CONFIG * BSX_active_confref_wk[SENSORLIST_INX_END - SENSORLIST_INX_WAKEUP_SIGNI_PRESSURE];
extern uint32_t active_nonwksensor_cnt;
extern uint32_t active_wksensor_cnt;

extern void get_sensor_t(int32_t sensor_id, struct sensor_t **pp_sensor_t, int32_t *p_list_inx);
extern int activate_configref_resort(int32_t bsx_list_index, int32_t is_enable);
extern int batch_configref_resort(int32_t bsx_list_index,
                int64_t sampling_period_ns, int64_t max_report_latency_ns, float delay_Hz);
extern int32_t convert_BSX_ListInx(int32_t bsx_list_inx);
extern void open_input_by_name(const char *event_name, int *p_fd, int *p_num);
extern void *hwcntl_main(void *arg);

extern int hwcntl_init(BoschSensor *boschsensor);

#endif


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

#ifndef ANDROID_BST_SENSOR_H
#define ANDROID_BST_SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <signal.h>
#include <pthread.h>
#if !defined(PLTF_LINUX_ENABLED)
#include <hardware/sensors.h>
#else
#include "sensors.h"
#endif
#include "sensord_def.h"
#include "boschsimple_list.h"

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    BoschSimpleList *p_list;
} SENSORD_SHARED_MEM;

class BoschSensor
{
public:
    static BoschSensor *getInstance();
    static void destroy();

    int activate(int handle, int enabled);
    int setDelay(int handle, int64_t ns);
    int read_events(sensors_event_t* data, int count);
    int batch(int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns);
    int flush(int handle);
#if defined(SENSORS_DEVICE_API_VERSION_1_4)
    int inject_sensor_data(const sensors_event_t *data);
#endif
    uint32_t get_sensorlist(struct sensor_t const** p_sSensorList);
    void sensord_read_rawdata();
    void sensord_deliver_event(sensors_event_t *p_event);

    int send_flush_event(int32_t sensor_id);

    int (*pfun_activate)(int handle, int enabled);
    int (*pfun_batch)(int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns);
    int (*pfun_flush)(BoschSensor *boschsensor, int handle);
    uint32_t (*pfun_get_sensorlist)(struct sensor_t const** p_sSensorList);
    uint32_t (*pfun_hw_deliver_sensordata)(BoschSensor *boschsensor);

    BoschSimpleList *tmplist_hwcntl_acclraw;
    BoschSimpleList *tmplist_hwcntl_gyroraw;
    BoschSimpleList *tmplist_hwcntl_magnraw;

    BoschSimpleList *tmplist_sensord_acclraw;
    BoschSimpleList *tmplist_sensord_gyroraw;
    BoschSimpleList *tmplist_sensord_magnraw;

    SENSORD_SHARED_MEM shmem_hwcntl;
    int HALpipe_fd[2];

private:
    BoschSensor();
    BoschSensor(const BoschSensor & other); //for cppcheck "noCopyConstructor"
    ~BoschSensor();

    static BoschSensor *instance;
    void sensord_cfg_init();

    pthread_t thread_sensord;
    pthread_t thread_hwcntl;
};

#endif  // ANDROID_BST_SENSOR_H

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
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include "BoschSensor.h"
#include "sensord.h"

#include "sensord_pltf.h"
#include "sensord_cfg.h"
#include "sensord_algo.h"
#include "sensord_hwcntl.h"
#include "axis_remap.h"

#include "util_misc.h"


void BoschSensor::sensord_read_rawdata()
{
    int ret;
    HW_DATA_UNION *p_hwdata;

    pthread_mutex_lock(&(shmem_hwcntl.mutex));

    if(0 == shmem_hwcntl.p_list->list_len)
    {
        ret = pthread_cond_wait(&(shmem_hwcntl.cond), &(shmem_hwcntl.mutex));
        if (ret)
        {
            pthread_mutex_unlock(&(shmem_hwcntl.mutex));
            return;
        }
    }
    /*	testing result and code investigation shows that even time out,
     pthread_cond_timedwait() will wait on calling pthread_mutex_lock(),
     so when return from pthread_cond_timedwait(), hwcntl may have sent
     the signal and unlock the pthread mutex at the same time occasionally,
     so check on share memory is still needed.
     When runs here, it means:
     1. 0 == ret, thread is woke up
     2. ETIMEDOUT == ret, but meanwhile hwcntl have sent the signal
     */

    while (shmem_hwcntl.p_list->list_len)
    {
        shmem_hwcntl.p_list->list_get_headdata((void **) &p_hwdata);

        switch (p_hwdata->id)
        {

            case SENSOR_TYPE_ACCELEROMETER:
                ret = tmplist_sensord_acclraw->list_add_rear((void *) p_hwdata);
                if (ret)
                {
                    PERR("list_add_rear() fail, ret = %d", ret);
                    if(-1 == ret){
                        free(p_hwdata);
                    }
                }
                break;
            case SENSOR_TYPE_GYROSCOPE_UNCALIBRATED:
                ret = tmplist_sensord_gyroraw->list_add_rear((void *) p_hwdata);
                if (ret)
                {
                    PERR("list_add_rear() fail, ret = %d", ret);
                    if(-1 == ret){
                        free(p_hwdata);
                    }
                }
                break;
            case SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED:
                ret = tmplist_sensord_magnraw->list_add_rear((void *) p_hwdata);
                if (ret)
                {
                    PERR("list_add_rear() fail, ret = %d", ret);
                    if(-1 == ret){
                        free(p_hwdata);
                    }
                }
                break;
        }
    }

    pthread_mutex_unlock(&(shmem_hwcntl.mutex));

    return;
}

void BoschSensor::sensord_deliver_event(sensors_event_t *p_event)
{
    int32_t ret;
    /*deliver up*/
    ret = write(HALpipe_fd[1], p_event, sizeof(sensors_event_t));
    if(ret < 0){
        PERR("deliver event fail, errno = %d(%s)", errno, strerror(errno));
    }

    free(p_event);

    return;
}


/**
 *
 * @param sensor_id
 * @return
 */
int BoschSensor::send_flush_event(int32_t sensor_id)
{
    sensors_meta_data_event_t *p_event;
    int32_t ret;

    p_event = (sensors_meta_data_event_t *) calloc(1, sizeof(sensors_meta_data_event_t));
    if (NULL == p_event)
    {
        PWARN("calloc fail");
        return -1;
    }

    p_event->version = META_DATA_VERSION;
    p_event->type = SENSOR_TYPE_META_DATA;
    p_event->meta_data.what = META_DATA_FLUSH_COMPLETE;
    p_event->meta_data.sensor = sensor_id;

    ret = write(HALpipe_fd[1], p_event, sizeof(sensors_meta_data_event_t));
    if(ret < 0){
        PERR("send flush echo fail, errno = %d(%s)", errno, strerror(errno));
    }

    free(p_event);

    return 0;
}


/**
 * Only used in AP solution
 */
void *sensord_main(void *arg)
{
    BoschSensor *bosch_sensor = reinterpret_cast<BoschSensor *>(arg);
    int ret = 0;

    while (1)
    {
        bosch_sensor->sensord_read_rawdata();
        sensord_algo_process(bosch_sensor);
    }

    //should not run here
    pthread_exit((void *) &ret);

    return NULL;
}

void sensord_sighandler(int signo, siginfo_t *sig_info, void *ctx)
{
    (void) sig_info;
    (void) ctx;
    int ret = 0;

    if (SIGTERM == signo)
    {
        pthread_exit((void *) &ret);
    }

    return;
}


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
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "BoschSensor.h"
#include "bsx_android.h"
#include "sensord_pltf.h"
#include "sensord_cfg.h"
#include "sensord_algo.h"
#include "sensord.h"
#include "sensord_hwcntl.h"
#include "util_misc.h"

static struct sigaction oldact;

static void sensord_sigact_enable()
{
    int ret = 0;
    struct sigaction sigact;

    memset(&sigact, 0, sizeof(struct sigaction));
    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = sensord_sighandler;

    ret = sigaction(SIGTERM, &sigact, &oldact);
    if (ret)
    {
        PERR("sigaction failed!");
    }

    return;
}

/**
 *
 */
BoschSensor::BoschSensor()
{
    int ret;

    pfun_activate = NULL;
    pfun_batch = NULL;
    pfun_flush = NULL;
    pfun_get_sensorlist = NULL;
    pfun_hw_deliver_sensordata = NULL;

    sensord_pltf_init();

    sensord_sigact_enable();

    sensord_cfg_init();

    ret = pipe(HALpipe_fd);
    if(0 != ret){
        PERR("create HAL pipe fail, errno = %d(%s)!", errno, strerror(errno));
        return;
    }
    fcntl(HALpipe_fd[0], F_SETFL, O_NONBLOCK);

    shmem_hwcntl.p_list = new BoschSimpleList();
    pthread_mutex_init(&shmem_hwcntl.mutex, NULL);
    pthread_cond_init(&shmem_hwcntl.cond, NULL);

    tmplist_sensord_acclraw = new BoschSimpleList();
    tmplist_sensord_gyroraw = new BoschSimpleList();
    tmplist_sensord_magnraw = new BoschSimpleList();

    tmplist_hwcntl_acclraw = new BoschSimpleList();
    tmplist_hwcntl_gyroraw = new BoschSimpleList();
    tmplist_hwcntl_magnraw = new BoschSimpleList();

    /**
     * Because hwcntl will also call algo library interface,
     * so the initialization must be finished before creating threads
     * BSX Lib initialization
     * */
    sensord_bsx_init();

    pthread_create(&thread_sensord, NULL, sensord_main, this);

    ret = hwcntl_init(this);
    if (ret){
        PERR("hwcntl_init_entry() fail, ret = %d!", ret);
        return;
    }

    pthread_create(&thread_hwcntl, NULL, hwcntl_main, this);

    return;
}

/**
 * for cppcheck "noCopyConstructor"
 * @param other
 */
BoschSensor::BoschSensor(const BoschSensor & other)
{
    *this = other;
}

BoschSensor::~BoschSensor()
{
    pthread_kill(thread_sensord, SIGTERM);
    pthread_kill(thread_hwcntl, SIGTERM);

    pthread_join(thread_sensord, NULL);
    pthread_join(thread_hwcntl, NULL);

    sigaction(SIGTERM, &oldact, NULL);

    pthread_mutex_destroy(&shmem_hwcntl.mutex);
    pthread_cond_destroy(&shmem_hwcntl.cond);
    delete shmem_hwcntl.p_list;

    close(HALpipe_fd[0]);
    close(HALpipe_fd[1]);

    sensord_pltf_clearup();
    if (bosch_sensorlist.list)
    {
        free(bosch_sensorlist.list);
    }

    if (bosch_sensorlist.bsx_list_index)
    {
        free(bosch_sensorlist.bsx_list_index);
    }

    delete tmplist_sensord_acclraw;
    delete tmplist_sensord_gyroraw;
    delete tmplist_sensord_magnraw;

    delete tmplist_hwcntl_acclraw;
    delete tmplist_hwcntl_gyroraw;
    delete tmplist_hwcntl_magnraw;
}

BoschSensor *BoschSensor::instance = NULL;
BoschSensor *BoschSensor::getInstance()
{
    if(NULL == instance)
    {
        instance = new BoschSensor();
    }

    return instance;
}

void BoschSensor::destroy()
{
    if(instance){
        delete instance;
        instance = NULL;
    }
}


/**
 *
 * @param p_sSensorList
 * @return
 */
uint32_t BoschSensor::get_sensorlist(struct sensor_t const** p_sSensorList)
{
    if(pfun_get_sensorlist){
        return pfun_get_sensorlist(p_sSensorList);
    }else{
        return 0;
    }
}

/**
 *
 * @param handle
 * @param enabled
 * @return
 */
int BoschSensor::activate(int handle, int enabled)
{
    if(pfun_activate){
        return pfun_activate(handle, enabled);
    }else{
        return 0;
    }
}

/**
 *
 * @param handle
 * @param flags
 * @param sampling_period_ns
 * @param max_report_latency_ns
 * @return
 */
int BoschSensor::batch(int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns)
{
    if(pfun_batch){
        return pfun_batch(handle, flags, sampling_period_ns, max_report_latency_ns);
    }else{
        return 0;
    }
}

/**
 *
 * @param handle
 * @param ns
 * @return
 */
int BoschSensor::setDelay(int handle, int64_t ns)
{
    return batch(handle, 0, ns, 0);
}

/**
 *
 * @param handle
 * @return
 */
int BoschSensor::flush(int handle)
{
    if(pfun_flush){
        return pfun_flush(this, handle);
    }else{
        return 0;
    }

}

#if defined(SENSORS_DEVICE_API_VERSION_1_4)
int BoschSensor::inject_sensor_data(const sensors_event_t *data)
{
    (void)data;
    return 0;
}
#endif


/**
 *
 * @param data
 * @param count
 * @return
 */
int BoschSensor::read_events(sensors_event_t* data, int count)
{
    int ret;

    ret = read(HALpipe_fd[0], data, count*sizeof(sensors_event_t));
    if(ret < 0)
    {
        PERR("read HAL pipe fail, errno = %d(%s)", errno, strerror(errno));
        return 0;
    }

    return (ret / sizeof(sensors_event_t));
}


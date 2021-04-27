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

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <poll.h>

#include "BoschSensor.h"
#include "bsx_android.h"

#include "sensors_poll_context.h"


/*****************************************************************************/
static const struct sensor_t *sSensorList = NULL;
static int sensorsNum = 0;

static int sensors__get_sensors_list(struct sensors_module_t* module,
        struct sensor_t const** list)
{
    (void) module;
    *list = sSensorList;
    return sensorsNum;
}

#if defined(SENSORS_DEVICE_API_VERSION_1_4)
static int sensors__set_operation_mode(unsigned int mode)
{
    (void)mode;
    return 0;
}
#endif


/*****************************************************************************/

sensors_poll_context_t::sensors_poll_context_t()
{
    bosch_sensor = BoschSensor::getInstance();
    sensorsNum = bosch_sensor->get_sensorlist(&sSensorList);
}

//for cppcheck "noCopyConstructor"
sensors_poll_context_t::sensors_poll_context_t(const sensors_poll_context_t & other)
{
    *this = other;
}

sensors_poll_context_t::~sensors_poll_context_t()
{
    BoschSensor::destroy();
}

int sensors_poll_context_t::activate(int handle, int enabled)
{
    return bosch_sensor->activate(handle, enabled);
}

int sensors_poll_context_t::setDelay(int handle, int64_t ns)
{
    return bosch_sensor->setDelay(handle, ns);
}

int sensors_poll_context_t::pollEvents(sensors_event_t* data, int count)
{
    int nbEvents = 0;
    int n = 0;
    int bosch_evncnt = 0;
    struct pollfd extended_mPollFds[1];

    extended_mPollFds[0].fd = bosch_sensor->HALpipe_fd[0];
    extended_mPollFds[0].events = POLLIN;
    extended_mPollFds[0].revents = 0;

    do {
        // see if we have some leftover from the last poll()
        if(extended_mPollFds[0].revents & POLLIN){
            bosch_evncnt = bosch_sensor->read_events(data, count);

            if (bosch_evncnt < count) {
                // no more data for next time
                extended_mPollFds[0].revents = 0;
            }
            count -= bosch_evncnt;
            nbEvents += bosch_evncnt;
            data += bosch_evncnt;
        }

        if (count) {
            // we still have some room, so try to see if we can get
            // some events immediately or just wait if we don't have
            // anything to return
            do {
                n = poll(extended_mPollFds, 1, nbEvents ? 0 : -1);
            } while (n < 0 && errno == EINTR);

            if (n < 0) {
                return -errno;
            }
        }
        // if we have events and space, go read them
    } while (n && count);

    return nbEvents;
}

int sensors_poll_context_t::batch(int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns)
{
    return bosch_sensor->batch(handle, flags, sampling_period_ns, max_report_latency_ns);
}

int sensors_poll_context_t::flush(int handle)
{
    return bosch_sensor->flush(handle);
}

#if defined(SENSORS_DEVICE_API_VERSION_1_4)
int sensors_poll_context_t::inject_sensor_data(const sensors_event_t *data)
{
    return bosch_sensor->inject_sensor_data(data);
}
#endif

/*****************************************************************************/
/*fixed route, just copy from other sensor HAL code*/
static int poll__close(struct hw_device_t *dev)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    if (ctx)
    {
        delete ctx;
    }
    return 0;
}

static int poll__activate(struct sensors_poll_device_t *dev,
        int handle, int enabled)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->activate(handle, enabled);
}

static int poll__setDelay(struct sensors_poll_device_t *dev,
        int handle, int64_t ns)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->setDelay(handle, ns);
}

static int poll__poll(struct sensors_poll_device_t *dev,
        sensors_event_t* data, int count)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->pollEvents(data, count);
}

static int poll__batch(struct sensors_poll_device_1 *dev,
        int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->batch(handle, flags, sampling_period_ns, max_report_latency_ns);
}

static int poll__flush(struct sensors_poll_device_1 *dev, int sensor_handle)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->flush(sensor_handle);
}

#if defined(SENSORS_DEVICE_API_VERSION_1_4)
static int poll__inject_sensor_data(struct sensors_poll_device_1 *dev, const sensors_event_t *data)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *) dev;
    return ctx->inject_sensor_data(data);
}
#endif


/*****************************************************************************/
/** Open a new instance of a sensor device using name */
static int open_sensors(const struct hw_module_t* module, const char* id,
        struct hw_device_t** device)
{
    int status = -EINVAL;
    sensors_poll_context_t *dev = new sensors_poll_context_t();
    (void) id;

    memset(&dev->device, 0, sizeof(struct sensors_poll_device_1));

    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = SENSORS_DEVICE_API_VERSION_1_3;
    dev->device.common.module = const_cast<hw_module_t*>(module);
    dev->device.common.close = poll__close;
    dev->device.activate = poll__activate;
    dev->device.setDelay = poll__setDelay;
    dev->device.poll = poll__poll;
    //for HAL versions >= SENSORS_DEVICE_API_VERSION_1_0
    dev->device.batch = poll__batch;
    dev->device.flush = poll__flush;
#if defined(SENSORS_DEVICE_API_VERSION_1_4)
    dev->device.inject_sensor_data = poll__inject_sensor_data;
#endif
    *device = &dev->device.common;
    status = 0;

    return status;
}

static struct hw_module_methods_t sensors_module_methods =
{
    .open = open_sensors
};

struct sensors_module_t HAL_MODULE_INFO_SYM =
{
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .version_major = 1,
        .version_minor = 0,
        .id = SENSORS_HARDWARE_MODULE_ID,
        .name = "Bosch Sensor module",
        .author = "Robert Bosch,GmbH",
        .methods = &sensors_module_methods,
        .dso = NULL,
        .reserved = {0}
    },
    .get_sensors_list = sensors__get_sensors_list,
#if defined(SENSORS_DEVICE_API_VERSION_1_4)
    .set_operation_mode = sensors__set_operation_mode,
#endif
};

#if defined(UNIT_TEST_ACTIVE)
#include "main.cpp"
#endif


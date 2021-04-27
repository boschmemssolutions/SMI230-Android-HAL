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


/*****************************************************************************/
struct sensors_poll_context_t
{
    struct sensors_poll_device_1 device; // must be first

    sensors_poll_context_t();
    //for cppcheck "noCopyConstructor"
    sensors_poll_context_t(const sensors_poll_context_t & other);
    ~sensors_poll_context_t();
    int activate(int handle, int enabled);
    int setDelay(int handle, int64_t ns);
    int pollEvents(sensors_event_t* data, int count);
    int batch(int handle, int flags, int64_t sampling_period_ns, int64_t max_report_latency_ns);
    int flush(int handle);
#if defined(SENSORS_DEVICE_API_VERSION_1_4)
    int inject_sensor_data(const sensors_event_t *data);
#endif

private:
    BoschSensor *bosch_sensor;
};

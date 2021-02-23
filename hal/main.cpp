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

int main()
{
    struct hw_module_t module;
    char id;
    struct hw_device_t* p_hw_device_t;
    sensors_poll_context_t *dev;
    int i;
#define MAX_EVENTS_READ 26
    sensors_event_t events[MAX_EVENTS_READ];
    int msg_cnt = 0;

    open_sensors(&module, &id, &p_hw_device_t);

    for (i = 0; i < sensorsNum; ++i) {
        printf("sSensorList[%d].name = %s, handle = %d \n",
                i, sSensorList[i].name, sSensorList[i].handle);
    }
    printf("sensorsNum = %d \n", sensorsNum);

    dev = (sensors_poll_context_t *)p_hw_device_t;
    for (i = 0; i < sensorsNum; ++i) {
        dev->device.activate((sensors_poll_device_t *)dev, sSensorList[i].handle, 0);
    }

    for (i = 0; i < sensorsNum; ++i) {
        dev->device.batch((sensors_poll_device_1 *)dev, sSensorList[i].handle, 0, 5000000, 0);
        dev->device.activate((sensors_poll_device_t *)dev, sSensorList[i].handle, 1);
    }

    while(1)
    {
        msg_cnt = dev->device.poll((sensors_poll_device_t *)dev, events, MAX_EVENTS_READ);
        for (i = 0; i < msg_cnt; ++i) {

            if(META_DATA_VERSION == events[i].version &&
                    (int)0xFFFFFFFF == events[i].meta_data.sensor)
            {
                printf("ending polling...\n");
                goto END;
            }else{
                printf("get event, sensor id = %d\n", events[i].sensor);
            }
        }

        printf("==========================\n\n");
    }

END:

    for (i = 0; i < sensorsNum; ++i) {
        dev->device.activate((sensors_poll_device_t *)dev, sSensorList[i].handle, 0);
    }

    delete(dev);

    return 0;
}

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

#include "axis_remap.h"

struct axis_remap_m
{
    unsigned int rx_y;
    unsigned int sx;
    unsigned int sy;
    unsigned int sz;
};

struct axis_remap_m axis_remap_matrix[8] = {
        /* rx_y sx sy sz */
        { 0, 0, 0, 0 }, /* P0 */
        { 0, 1, 0, 1 }, /* P1 */
        { 0, 1, 1, 0 }, /* P2 */
        { 0, 0, 1, 1 }, /* P3 */
        { 1, 1, 0, 0 }, /* P4 */
        { 1, 0, 1, 0 }, /* P5 */
        { 1, 0, 0, 1 }, /* P6 */
        { 1, 1, 1, 1 }, /* P7 */
};

void hw_remap_sensor_data(float *px, float *py, float *pz, int position)
{
    float swap_tmp;

    if (axis_remap_matrix[position].rx_y)
    {
        swap_tmp = *px;
        *px = *py;
        *py = swap_tmp;
    }

    if (axis_remap_matrix[position].sx)
    {
        *px = 0 - *px;
    }

    if (axis_remap_matrix[position].sy)
    {
        *py = 0 - *py;
    }

    if (axis_remap_matrix[position].sz)
    {
        *pz = 0 - *pz;
    }

    return;
}


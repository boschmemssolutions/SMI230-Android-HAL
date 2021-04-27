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

#ifndef BOSCH_SIMPLE_LIST_H
#define BOSCH_SIMPLE_LIST_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdint.h>

struct list_node
{
    struct list_node *next;
    void* p_data;
};

struct dlist_node
{
    struct dlist_node *prev;
    struct dlist_node *next;
    long data;
};

class BoschSimpleList
{
public:
    BoschSimpleList();
    ~BoschSimpleList();

    void set_uplimit(uint32_t limit);
    int list_add_rear(void *pdata);
    void list_get_headdata(void **ppdata);
    int list_mount_rear(BoschSimpleList *list_for_mnt);
    int list_clean();

    struct list_node *head;
    struct list_node *tail;
    uint32_t list_len;

private:
    uint32_t uplimit;
};

#endif

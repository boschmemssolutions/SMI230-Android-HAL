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

#include "boschsimple_list.h"
#include "sensord_pltf.h"

#define DEFAULT_LIST_LEN 128

BoschSimpleList::BoschSimpleList()
{
    head = NULL;
    tail = NULL;
    list_len = 0;
    set_uplimit(DEFAULT_LIST_LEN);

    return;
}

BoschSimpleList::~BoschSimpleList()
{
    list_clean();
    return;
}

void BoschSimpleList::set_uplimit(uint32_t limit)
{
    if (0 == limit)
    {
        //at least 1 node can be stored
        // to log something...
        limit = 1;
    }
    uplimit = limit;
}

int BoschSimpleList::list_add_rear(void *pdata)
{
    struct list_node *nod;
    void *del;
    int ret = 0;

    nod = (struct list_node *) malloc(sizeof(struct list_node));
    if (NULL == nod)
    {
        return -1;
    }

    if (list_len == uplimit)
    {
        ret = -2;
        PERR("list buffer is full, drop the oldest data");
        list_get_headdata(&del);
        free(del);
    }

    nod->p_data = pdata;
    nod->next = NULL;
    if (NULL == head)
    {
        /*to be convenient, when running,
         tail point is allowed to have obsolete value.
         so use head point to judge if empty
         */
        //first node added
        tail = nod;
        head = nod;
    }
    else
    {
        tail->next = nod;
        tail = nod;
    }
    list_len++;

    return ret;
}

void BoschSimpleList::list_get_headdata(void **ppdata)
{
    struct list_node *cur;

    if (0 == list_len)
    {
        *ppdata = NULL;
        return;
    }

    *ppdata = head->p_data;
    cur = head;
    head = head->next;
    list_len--;

    free(cur);

    return;
}

int BoschSimpleList::list_mount_rear(BoschSimpleList *list_for_mnt)
{
    void *pdata = NULL;
    int ret = 0;

    if (NULL == list_for_mnt || 0 == list_for_mnt->list_len)
    {
        return 0;
    }

    if (NULL == head)
    {
        /*to be convenient, when running,
         tail point is allowed to have obsolete value.
         so use head point to judge if empty
         */
        //the destined list is yet empty
        head = list_for_mnt->head;
        tail = list_for_mnt->tail;
    }
    else
    {
        tail->next = list_for_mnt->head;
        tail = list_for_mnt->tail;
    }
    list_len += list_for_mnt->list_len;

    list_for_mnt->head = NULL;
    list_for_mnt->tail = NULL;
    list_for_mnt->list_len = 0;

    //truncate to uplimit
    while (list_len > uplimit)
    {
        ret = -1;
        PERR("add too much, drop the oldest data");
        list_get_headdata(&pdata);
        free(pdata);
    }

    return ret;
}

int BoschSimpleList::list_clean()
{
    void *pdata = NULL;

    while (list_len)
    {
        list_get_headdata(&pdata);
        free(pdata);
    }

    return 0;
}


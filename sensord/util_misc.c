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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>

#include "util_misc.h"

void get_token_pairs(const char *psz_line, char seperator, char *ptoken_l, char *ptoken_r)
{
    char ch;
    int i;
    int pos = -1;

    i = 0;
    while ('\0' != (ch = psz_line[i]))
    {
        if (ch == seperator)
        {
            pos = i;
            break;
        }
        else
        {
            ptoken_l[i] = psz_line[i];
        }
        i++;
    }
    ptoken_l[i] = '\0';

    if (-1 != pos)
    {
        i = pos + 1;
        while ('\0' != psz_line[i])
        {
            ptoken_r[i - pos - 1] = psz_line[i];
            i++;
        }
        ptoken_r[i - pos - 1] = '\0';
    }
    else
    {
        ptoken_r[0] = '\0';
    }
}

/*! get greatest common dividor
 */
int get_gcd(int a, int b)
{
    if (0 == b)
        return a;
    return get_gcd(b, a % b);
}

int32_t get_nearest_divisible_int(uint32_t a, uint32_t b)
{
    int32_t ret;

    if (0 == b)
    {
        /* error */
        return -1;
    }

    if (0 == a)
    {
        return (int32_t) b;
    }

    if (a <= b)
    {
        return (int32_t) a;
    }

    for (ret = b; ret > 0; ret--)
    {
        if (0 == (a % ret))
        {
            break;
        }
    }

    return ret;
}


void debug_mem(uint8_t *addr, uint32_t len)
{
    unsigned int i;
    unsigned int BYTES_PER_LINE = 16;

    for (i = 0; i < len; i++)
    {
        printf("%02x%c", addr[i], ((i + 1) % BYTES_PER_LINE) ? ' ' : '\n');
    }

    printf("\n+++++++++++++++++++++++++++++++++\n");
}


/*msecs is assumed to be in milliseconds */
int bosch_pthread_mutex_timedlock(pthread_mutex_t *mutex, uint32_t msecs)
{
#if 1
#define INTERVAL_USEC 1000
    useconds_t usecs;
    int ret = 0;

    usecs = msecs * 1000;
    while (usecs)
    {
        ret = pthread_mutex_trylock(mutex);
        if (0 == ret)
        {
            return 0;
        }

        usecs -= INTERVAL_USEC;
        usleep(INTERVAL_USEC);
    }

    return ret;

#else //So far pthread_mutex_timedlock is not provided in AOSP version
    struct timespec t_set;
    struct timeval time_now;
    time_t s_set;
    long us_set;

    gettimeofday(&time_now, NULL);
    s_set = time_now.tv_sec + msecs / 1000;
    us_set = time_now.tv_usec + (msecs%1000)*1000;
    //carry up
    s_set += us_set/1000000;
    us_set %= 1000000;

    t_set.tv_sec = s_set;
    t_set.tv_nsec = us_set * 1000;

    return pthread_mutex_timedlock(mutex, &t_set);
#endif
}

uint32_t sensord_popcount_32(uint32_t x)
{
    const uint32_t m1 = 0x55555555; //binary: 0101...
    const uint32_t m2 = 0x33333333; //binary: 00110011..
    const uint32_t m4 = 0x0f0f0f0f; //binary:  4 zeros,  4 ones ...
    const uint32_t m8 = 0x00ff00ff; //binary:  8 zeros,  8 ones ...
    const uint32_t m16 = 0x0000ffff; //binary: 16 zeros, 16 ones ...
    //const uint32_t hff = 0xffffffff; //binary: all ones
    //const uint32_t h01 = 0x01010101; //the sum of 256 to the power of 0,1,2,3...

    x = (x & m1) + ((x >> 1) & m1); //put count of each  2 bits into those  2 bits
    x = (x & m2) + ((x >> 2) & m2); //put count of each  4 bits into those  4 bits
    x = (x & m4) + ((x >> 4) & m4); //put count of each  8 bits into those  8 bits
    x = (x & m8) + ((x >> 8) & m8); //put count of each 16 bits into those 16 bits
    x = (x & m16) + ((x >> 16) & m16); //put count of each 32 bits into those 32 bits

    return x;
}

uint32_t sensord_popcount_64(uint64_t x)
{
    const uint64_t m1 = 0x5555555555555555; //binary: 0101...
    const uint64_t m2 = 0x3333333333333333; //binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f; //binary:	4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff; //binary:	8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
    //const uint64_t hff = 0xffffffffffffffff; //binary: all ones
    //const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

    x = (x & m1) + ((x >> 1) & m1); //put count of each  2 bits into those  2 bits
    x = (x & m2) + ((x >> 2) & m2); //put count of each  4 bits into those  4 bits
    x = (x & m4) + ((x >> 4) & m4); //put count of each  8 bits into those  8 bits
    x = (x & m8) + ((x >> 8) & m8); //put count of each 16 bits into those 16 bits
    x = (x & m16) + ((x >> 16) & m16); //put count of each 32 bits into those 32 bits
    x = (x & m32) + ((x >> 32) & m32); //put count of each 64 bits into those 64 bits

    return x;
}

/* As Wegner (1960) described, the bitwise and of x with x-1 differs
 from x only in zeroing out the least significant nonzero bit: subtracting 1
 changes the rightmost string of 0s to 1s, and changes the rightmost 1 to a 0.
 If x originally had n bits that were 1, then after only n iterations of this operation,
 x will be reduced to zero */
uint32_t sensord_popcount_less1(uint64_t x)
{
    uint32_t count;

    for (count = 0; x; count++)
        x &= x - 1;

    return count;
}


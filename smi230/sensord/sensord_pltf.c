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

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "sensord_def.h"
#include "sensord_cfg.h"
#include "sensord_pltf.h"

#define SENSORD_TRACE_FILE (PATH_DIR_SENSOR_STORAGE "/sensord.log")
#define DATA_IN_FILE (PATH_DIR_SENSOR_STORAGE "/data_in.log")
#define BSX_DATA_LOG (PATH_DIR_SENSOR_STORAGE "/bsx_datalog.log")

static FILE *g_fp_trace = NULL;
static FILE *g_dlog_input = NULL;
static FILE *g_bsx_dlog = NULL;

static inline void storage_init()
{
    char *path = NULL;
    int ret = 0;
    struct stat st;

    path = (char *) (PATH_DIR_SENSOR_STORAGE);

    ret = stat(path, &st);
    if (0 == ret)
    {
        if (S_IFDIR == (st.st_mode & S_IFMT))
        {
            /*already exist*/
            ret = chmod(path, 0766);
            if (ret)
            {
                printf("error chmod on %s", path);
            }
        }

        return;
    }

    ret = mkdir(path, 0766);
    if (ret)
    {
        printf("error creating storage dir\n");
    }
    chmod(path, 0766); //notice the "umask" could mask some privilege when mkdir

    return;
}

void sensord_trace_init()
{
    g_fp_trace = fopen(SENSORD_TRACE_FILE, "w");
    if(NULL == g_fp_trace)
    {
        printf("sensord_trace_init: fail to open log file %s! \n", SENSORD_TRACE_FILE);
        g_fp_trace = stdout;
        return;
    }

    chmod(SENSORD_TRACE_FILE, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

    return;
}

int64_t sensord_get_tmstmp_ns(void)
{

    int64_t ap_time;
    struct timespec ts;

    clock_gettime(CLOCK_BOOTTIME, &ts);
    ap_time = (int64_t) ts.tv_sec * 1000000000 + ts.tv_nsec;

    return ap_time;

}

void trace_log(uint32_t level, const char *fmt, ...)
{
    int ret = 0;
    va_list ap;
#if !defined(PLTF_LINUX_ENABLED)
    char buffer[256] = { 0 };
#endif

    if (0 == trace_to_logcat)
    {
        if (0 == (trace_level & level))
        {
            return;
        }

        va_start(ap, fmt);
        ret = vfprintf(g_fp_trace, fmt, ap);
        va_end(ap);

        // otherwise, data is buffered rather than be wrote to file
        // therefore when stopped by signal, NO data left in file!
        fflush(g_fp_trace);

        if (ret < 0)
        {
            printf("trace_log: fprintf(g_fp_trace, fmt, ap)  fail!!\n");
        }
    }
    else
    {

#if !defined(PLTF_LINUX_ENABLED)
        /**
         * here use android api
         * Let it use Android trace level.
         */
#include<android/log.h>
#define BST_LOG_TAG    "sensord"

        va_start(ap, fmt);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, ap);
        va_end(ap);

        switch (level)
        {
            case LOG_LEVEL_N:
                __android_log_print(ANDROID_LOG_FATAL, BST_LOG_TAG, "%s", buffer);
                break;
            case LOG_LEVEL_E:
                __android_log_print(ANDROID_LOG_ERROR, BST_LOG_TAG, "%s", buffer);
                break;
            case LOG_LEVEL_W:
                __android_log_print(ANDROID_LOG_WARN, BST_LOG_TAG, "%s", buffer);
                break;
            case LOG_LEVEL_I:
                __android_log_print(ANDROID_LOG_INFO, BST_LOG_TAG, "%s", buffer);
                break;
            case LOG_LEVEL_D:
                __android_log_print(ANDROID_LOG_DEBUG, BST_LOG_TAG, "%s", buffer);
                break;
            case LOG_LEVEL_LADON:
                __android_log_print(ANDROID_LOG_WARN, BST_LOG_TAG, "%s", buffer);
                break;
            default:
                break;
        }

#else

        if(0 == (trace_level & level))
        {
            return;
        }

        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);

#endif
    }

    return;
}

static void generic_data_log(const char*dest_path, FILE **p_dest_fp, char *info_str)
{
    if (NULL == (*p_dest_fp))
    {
        (*p_dest_fp) = fopen(dest_path, "w");
        if (NULL == (*p_dest_fp))
        {
            printf("fail to open file %s! \n", dest_path);
            return;
        }

        chmod(dest_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }

    fprintf((*p_dest_fp), "%s", info_str);

    // otherwise, data is buffered rather than be wrote to file
    // therefore when stopped by signal, NO data left in file!
    fflush((*p_dest_fp));

}

void data_log_algo_input(char *info_str)
{
    generic_data_log(DATA_IN_FILE, &g_dlog_input, info_str);
}

void bsx_datalog_algo(char *info_str)
{
    generic_data_log(BSX_DATA_LOG, &g_bsx_dlog, info_str);
}


void sensord_pltf_init(void)
{
    storage_init();

    sensord_trace_init();

    return;
}

void sensord_pltf_clearup(void)
{
    fclose(g_fp_trace);

    if (g_dlog_input)
    {
        fclose(g_dlog_input);
    }
    if(g_bsx_dlog)
    {
        fclose(g_bsx_dlog);
    }
    return;
}


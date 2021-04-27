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

#ifndef __SENSORD_PLTF_H
#define __SENSORD_PLTF_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_LEVEL_LADON 0x20 //for LADON
#define LOG_LEVEL_N 0x10 //note
#define LOG_LEVEL_E 0x8 //err
#define LOG_LEVEL_W 0x4 //warning
#define LOG_LEVEL_I 0x2 //info
#define LOG_LEVEL_D 0x1 //debug


#define GET_TIME_TICK() (sensord_get_tmstmp_ns())
//#define GET_TIME_TICK() (0)

#define BS_LOG trace_log

#define BS_LOG_FORMAT(fmt,type) "[%lld]%s(%s Ln%d) " fmt "\n", \
			GET_TIME_TICK(),type, __FILE__, __LINE__

#define LADON_LOG_FORMAT(fmt,type) "[%lld]%s" fmt "\n", \
            GET_TIME_TICK(),type

#define PLADON(fmt, args...) BS_LOG(LOG_LEVEL_LADON, LADON_LOG_FORMAT(fmt, "[LADON]"), ##args)

#define PNOTE(fmt, args...) BS_LOG(LOG_LEVEL_N, BS_LOG_FORMAT(fmt, "[NOTE]"), ##args)

#define PERR(fmt, args...) BS_LOG(LOG_LEVEL_E, BS_LOG_FORMAT(fmt, "[ERR]"), ##args)

#define PWARN(fmt, args...) BS_LOG(LOG_LEVEL_W, BS_LOG_FORMAT(fmt, "[WARN]"), ##args)

#define PINFO(fmt, args...) BS_LOG(LOG_LEVEL_I, BS_LOG_FORMAT(fmt, "[INFO]"), ##args)

#define PDEBUG(fmt, args...) BS_LOG(LOG_LEVEL_D, BS_LOG_FORMAT(fmt, "[DEBUG]"), ##args)

extern int64_t sensord_get_tmstmp_ns(void);
extern void trace_log(uint32_t level, const char *fmt, ...);
extern void sensord_pltf_init(void);
extern void sensord_pltf_clearup(void);
extern void data_log_algo_input(char *info_str);
extern void bsx_datalog_algo(char *info_str);

#ifdef __cplusplus
}
#endif

#endif

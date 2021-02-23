/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Robert Bosch GmbH. All rights reserved. 
 * Copyright (C) 2011~2015 Bosch Sensortec GmbH All Rights Reserved
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

/*!@addtogroup bsx_integrationsupport
 * @{*/

#ifndef __BSX_USER_DEF_H__
#define __BSX_USER_DEF_H__

/*!
 * @brief Contains the user specific definitions to be included with bsx_interface.h
 *
 * @note this shall typically be changeable and changed by a user, e.g. to include sensors.h from the Android headers
 */

#ifdef __cplusplus
extern "C"
{
#endif

//#include "sensors.h" // locate here e.g. sensors.h from Android

#ifndef BSX_TRACE_ENABLED
#define BSX_TRACE_ENTRY(object, event, id)
#else
#define BSX_TRACE_ENTRY(object, event, id)
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __BSX_USER_DEF_H__ */

/*!@}*/

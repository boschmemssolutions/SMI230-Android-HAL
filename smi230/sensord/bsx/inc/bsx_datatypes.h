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

#ifndef __BSX_DATATYPES_H__
#define __BSX_DATATYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif


/*! defines the various platforms  */
#define BSX_PLATFORM_ANDROID    (1) /**< defines android platform  */
#define BSX_PLATFORM_WINDOWS    (2) /**< defines windows platform  */

/*! defines the various machine bit configuration */
#define BSX_MACHINE_16BIT         (1) /**< defines 16bit configuration  */
#define BSX_MACHINE_32BIT         (2) /**< defines 32bit configuration  */
#define BSX_MACHINE_64BIT         (3) /**< defines 64bit configuration  */

/*! defines the various compiler configuration */
#define BSX_COMPILER_STDC_C11     (1) /**< defines standard c compiler c11 version  */
#define BSX_COMPILER_STDC_C99     (2) /**< defines standard c compiler c99 and higher version  */
#define BSX_COMPILER_OTHER         (255) /**< defines all other supported compiers  */

/*! defines bsx platform as android by default  */
#ifndef BSX_PLATFORM
#define BSX_PLATFORM              (BSX_PLATFORM_ANDROID)
#endif

/*! defines bsx machine as 32 bit by default  */
#ifndef BSX_MACHINE
#define BSX_MACHINE              (BSX_MACHINE_32BIT)
#endif

/*! @brief includes */

#if (BSX_MACHINE == BSX_MACHINE_16BIT)
#include <limits.h>
#endif

/*! @brief typedefs for basic data types */

#ifdef __STDC__
/*! C11 - C standard */
#if (__STDC_VERSION__ == 201112L)
#define BSX_COMPILER    (BSX_COMPILER_STDC_C11)

#include <stdint.h>

/*! unsigned integer types */
typedef uint8_t bsx_u8_t; /**< defines bsx type for unsigned 8bit integer  */
typedef uint16_t bsx_u16_t; /**< defines bsx type for unsigned 16bit integer  */
typedef uint32_t bsx_u32_t; /**< defines bsx type for unsigned 32bit integer  */
typedef uint64_t bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */

/*!signed integer types */
typedef int8_t bsx_s8_t;
typedef int16_t bsx_s16_t;
typedef int32_t bsx_s32_t;
typedef int64_t bsx_s64_t;

/*!size types */
typedef size_t bsx_size_t;

typedef int bsx_bool_t; /**< boolean type */

#define BSX_FALSE (0)        /**< defines the use of false  */
#define BSX_TRUE (1)        /**< defines the use of true  */

/*! C99 - C standard and higher */
#elif (__STDC_VERSION__ >= 199901L)
#define BSX_COMPILER    (BSX_COMPILER_STDC_C99)

#include <stddef.h>
#include <stdint.h>

/*! unsigned integer types */
typedef uint8_t bsx_u8_t; /**< defines bsx type for unsigned 8bit integer  */
typedef uint16_t bsx_u16_t; /**< defines bsx type for unsigned 16bit integer  */
typedef uint32_t bsx_u32_t; /**< defines bsx type for unsigned 32bit integer  */
typedef uint64_t bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */

/*! signed integer types */
typedef int8_t bsx_s8_t;
typedef int16_t bsx_s16_t;
typedef int32_t bsx_s32_t;
typedef int64_t bsx_s64_t;

/*! @brief size types */
typedef size_t bsx_size_t;

typedef int bsx_bool_t; /**< boolean type */

#define BSX_FALSE (0U)        /**< defines the use of false  */
#define BSX_TRUE  (1U)        /**< defines the use of true  */

/*! C89 or other - C standard */
#else
#define BSX_COMPILER    (BSX_COMPILER_OTHER)
#endif
#else
#define BSX_COMPILER    (BSX_COMPILER_OTHER)
#endif

#if (BSX_COMPILER == BSX_COMPILER_OTHER)
#if (BSX_MACHINE == BSX_MACHINE_16BIT)

/*! unsigned integer types */
typedef unsigned char bsx_u8_t; /**< defines bsx type for unsigned 8bit integer  */
typedef unsigned short int bsx_u16_t; /**< defines bsx type for unsigned 16bit integer  */
typedef unsigned long int bsx_u32_t; /**< defines bsx type for unsigned 32bit integer  */

/*! signed integer types */
typedef signed char bsx_s8_t;
typedef signed short int bsx_s16_t;
typedef signed long int bsx_s32_t;

#ifdef (LONG_MAX) && LONG_MAX == 0x7fffffffffffffffL
typedef long int bsx_s64_t;
typedef unsigned long int bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */
#elif defined(LLONG_MAX) && (LLONG_MAX == 0x7fffffffffffffffLL)
typedef long long int bsx_s64_t;
typedef unsigned long long int bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */
#else
#warning If 64 bit integers are supported on your platform, \
    define bsx_s64_t/bsx_u64_t manually.
#endif

#elif (BSX_MACHINE == BSX_MACHINE_32BIT)

/*! unsigned integer types*/
typedef unsigned char bsx_u8_t; /**< defines bsx type for unsigned 8bit integer  */
typedef unsigned short int bsx_u16_t; /**< defines bsx type for unsigned 16bit integer  */
typedef unsigned int bsx_u32_t; /**< defines bsx type for unsigned 32bit integer  */
typedef unsigned long long int bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */

/*! signed integer types*/
typedef signed char bsx_s8_t; /**< defines bsx type for signed 8bit integer  */
typedef signed short int bsx_s16_t; /**< defines bsx type for signed 16bit integer  */
typedef signed int bsx_s32_t; /**< defines bsx type for signed 32bit integer  */
typedef signed long long int bsx_s64_t; /**< defines bsx type for signed 64bit integer  */


#elif (BSX_MACHINE == BSX_MACHINE_64BIT)

/*unsigned integer types*/
typedef unsigned char bsx_u8_t; /**< defines bsx type for unsigned 8bit integer  */
typedef unsigned short int bsx_u16_t; /**< defines bsx type for unsigned 16bit integer  */
typedef unsigned int bsx_u32_t; /**< defines bsx type for unsigned 32bit integer  */
typedef unsigned long long int bsx_u64_t; /**< defines bsx type for unsigned 64bit integer  */

/*signed integer types*/
typedef signed char bsx_s8_t;
typedef signed short int bsx_s16_t;
typedef signed int bsx_s32_t;
typedef signed long long int bsx_s64_t;

#else
#warning machine configuration not supported
#endif

typedef unsigned long int bsx_size_t; /**< size types */

#define BSX_FALSE (0)        /**< defines the use of false  */
#define BSX_TRUE (1)        /**< defines the use of true  */

#endif

/*! @brief Additional basic data types */
typedef float bsx_f32_t;
typedef double bsx_f64_t;   /**< float type */
typedef bsx_u8_t bsx_char_t; /**< char type */

typedef int bsx_bool_t; /**< boolean type */

#ifdef __FUSER__
#define BSX_CONST_DECL const /**< chess_storage(aROMb) */
#else
#define BSX_CONST_DECL const    /**< chess_storage(aROMb) */
#endif

/*! @brief Handling External Data types for signed 64 bit */

typedef bsx_s64_t bsx_ts_external_t;

/*! @brief structure definition to hold the version information of the software */
typedef struct
{
    bsx_u8_t version_major; /**< major version */
    bsx_u8_t version_minor; /**< minor version */
    bsx_u8_t bugfix_major; /**< major bug-fix identification */
    bsx_u8_t bugfix_minor; /**< minor bug-fix identification */
} bsx_version_t;

/*!
 * @brief Definition of the pointer to the BSX4 library
 */
typedef bsx_u8_t bsx_instance_t;


/*!
 * @brief Content of one dimension of a frame in a FIFO data set
 * \note Supports only 32 bit values.
 */
typedef union
{
    bsx_f64_t lfp; /**< long floating-point access  */
    bsx_u64_t ulli; /**< unsigned long long access */
    bsx_s64_t slli; /**< signed long long  access */
    struct {
        union {
            bsx_f32_t sfp; /**< floating-point access  */
            bsx_s32_t sli; /**< signed integer access */
            bsx_u32_t uli; /**< unsigned integer access */
            struct
            {
                bsx_s16_t value; /**< signed integer value */
                bsx_s16_t reserved; /**< reserved */
            } ssi; /**< signed short integer access */
            struct
            {
                bsx_u16_t value; /**< unsigned integer value */
                bsx_u16_t reserved; /**< reserved */
            } usi; /**< unsigned short integer access */
            struct
            {
                bsx_s8_t value; /**< signed integer value */
                bsx_s8_t reserved0; /**< reserved */
                bsx_s16_t reserved1; /**< reserved */
            } sti; /**< signed short integer access */
            struct
            {
                bsx_u8_t value; /**< unsigned integer value */
                bsx_u8_t reserved0; /**< reserved */
                bsx_u16_t reserved1; /**< reserved */
            } uti; /**< unsigned short integer access */
        } mslw; /**< most significant 32bit word of the long word (64bit) */
        bsx_u32_t reserved; /**< least significant 32bit word of the long word (64bit) */
    } lw; /**< long word (64bit) */
} bsx_data_content_t;

/*! @brief BSX specific return value */
typedef bsx_s32_t bsx_return_t;

/*!
 * @brief Storage for a FIFO data set using frames
 *
 * One frame consists of \p nDim values. A set consists of \p nLength frames.
 */
typedef struct
{
    bsx_ts_external_t time_stamp; /**< time stamp  */
    bsx_data_content_t *content_p; /**< content of the FIFO set   */
    bsx_s32_t data_type; /**< data type used within the frames of the FIFO set */
    bsx_u16_t sensor_id; /**< identifier of the virtual or physical sensor  */
    bsx_u8_t dims; /**< number of dimensions in a frame  */
    bsx_u8_t depth; /**< number of frames in a FIFO set */
} bsx_fifo_data_t;

/*! @brief Configuration of a virtual or physical sensor */
typedef struct
{
    bsx_u32_t sensor_id;                /*!< identifier of the virtual or physical sensor */
    bsx_f32_t sample_rate;              /*!< sample rate of the virtual or physical sensor */
} bsx_sensor_configuration_t;

/*! @brief Create one function local buffer
 *
 * @note If used within a function, the stack size may be increased tremendously depending on the
 *       dimensions and frames. Use \p BSX_CREATE_FIFO_GLOBAL to allocate globally in RAM instead
 *       of putting it on the stack.
 */
#define BSX_CREATE_FIFO_LOCAL(name, type, n_dims, n_frames)  \
bsx_data_content_t bsx_##name##_buffer[(n_dims)*(n_frames)]; \
bsx_fifo_data_t bsx_##name =                                 \
{                                                            \
        0,                                                   \
        (bsx_##name##_buffer),                               \
        (type),                                              \
        0U,                                                  \
        (n_dims),                                            \
        (n_frames)                                           \
};                                                           \

/*! @brief Create a global buffer
 *
 * @note Increases the RAM consumption but does affect the stack size. Use \p BSX_CREATE_FIFO_LOCAL to
 *       avoid allocating global variabes. */
#define BSX_CREATE_FIFO_GLOBAL(name, type, n_dims, n_frames)   \
bsx_data_content_t bsx_##name##_buffer_g[(n_dims)*(n_frames)]; \
bsx_fifo_data_t bsx_##name##_g =                               \
{                                                              \
        0,                                                     \
        (bsx_##name##_buffer_g),                               \
        (type),                                                \
        0U,                                                    \
        (n_dims),                                              \
        (n_frames)                                             \
};

/*! @brief Create many function local buffers
 *
 * @note If used within a function, the stack size may be increased tremendously depending on the
 *       dimensions and frames. Use \p BSX_CREATE_FIFO_GLOBAL to allocate globally in RAM instead
 *       of putting it on the stack.
 */
#define BSX_CREATE_FIFOS_LOCAL(name, count, type, n_dims, n_frames)       \
bsx_data_content_t bsx_##name##_buffer[(count)][(n_dims)*(n_frames)];     \
bsx_fifo_data_t bsx_##name[count];                                        \
{                                                                         \
    bsx_size_t n_buffer = sizeof(bsx_data_content_t)*(n_dims)*(n_frames); \
    for (bsx_size_t idx = 0U; idx < count; ++idx)                         \
    {                                                                     \
        bsx_##name[idx].time_stamp = 0U;                                  \
        bsx_##name[idx].content_p = (bsx_##name##_buffer)[idx];           \
        memset(bsx_##name##_buffer[idx], 0x0U, n_buffer);                 \
        bsx_##name[idx].data_type = (type);                               \
        bsx_##name[idx].sensor_id = BSX_SENSOR_ID_INVALID;                \
        bsx_##name[idx].dims = (n_dims);                                  \
        bsx_##name[idx].depth = (n_frames);                               \
    }                                                                     \
}


/*! @brief Identifiers for data types
 *
 * Allow computation of sizes during run-time for buffers hidden by pointers.
 */
typedef enum
{
    BSX_BUFFER_TYPE_FHUGE, /**< double precision floating-point as defined within IEEE-754 */
    BSX_BUFFER_TYPE_S64, /**< signed 64 bit integer */
    BSX_BUFFER_TYPE_U64, /**< unsigned 64 bit integer */
    BSX_BUFFER_TYPE_FLONG, /**< single precision floating-point as defined within IEEE-754 */
    BSX_BUFFER_TYPE_S32, /**< signed 32 bit integer */
    BSX_BUFFER_TYPE_U32, /**< unsigned 32 bit integer */
    BSX_BUFFER_TYPE_S16, /**< signed 16 bit integer */
    BSX_BUFFER_TYPE_U16, /**< unsigned 16 bit integer */
    BSX_BUFFER_TYPE_S8, /**< signed 8 bit integer */
    BSX_BUFFER_TYPE_U8, /**< unsigned 8 bit integer */
    BSX_BUFFER_TYPE_UNKNOWN
} bsx_buffer_datatype_identifier_t;


/*! @brief Thread identifiers for grouping of channel processing
 *
 * @{
 */
#define BSX_THREAD_MAIN           (0x01U) /**< Identifier for the main thread */
#define BSX_THREAD_CALIBRATION    (0x02U) /**< Identifier for the thread running self-calibration */
/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* __BSX_DATATYPES_H__ */
/*! @}*/

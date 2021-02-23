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

/*!
 * @defgroup bsx    BSX API
 * BSX application programming interface
 * @{
 *    Provide here the documentation for the BSX API
 * */

#ifndef __BSX_LIBRARY_H__
#define __BSX_LIBRARY_H__

#include "bsx_datatypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Retrieve the current version of the library
 *
 * @param[in,out]      bsx_version_p       Pointer to structure containing the version information
 *
 * @return Zero when successful, otherwise an error code
 *  @retval BSX_OK
 */
bsx_return_t bsx_get_version(bsx_version_t * bsx_version_p);

/*! @brief Initialize the library
 *
 * @param[in,out]   bsx_p                           Instance of the library
 *
 * @return Zero when successful, otherwise an error code
 *  @retval BSX_OK
 *  @retval BSX_E_INVALIDSTATE
 *
 * @note During initialization with this function, the instance of the library is already set 
 *       to defaults by \p bsx_reset .
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesInit "initialization of BSX".
 */
bsx_return_t bsx_init(bsx_instance_t * const bsx_p);

/*! @brief perform signal processing steps of the library for provided signal samples and, if desired, return results of the processing
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       threads                         Identifiers of the threads to be executed
 * @param[in]       data_in_p                       Array of input signals with signal samples
 * @param[in]       n_data_in                       Number of elements to be used \p data_in_p
 * @param[out]      data_out_p                      Array of output signals with signal samples. If not required, pass NULL.
 * @param[in,out]   n_data_out_p                    On calling the function: maximum length of \p data_out_p or zero if nothing shall be written to \p data_out_p
 *                                                  On function return: number of returned outputs
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_I_DOSTEPS_NOOUTPUTSRETURNED
 *  @retval BSX_I_DOSTEPS_NOOUTPUTSRETURNABLE
 *  @retval BSX_W_DOSTEPS_EXCESSOUTPUTS means that further results shall be read using bsx_get_output_signal. If unsure which virtual
 *          sensor outputs are currently enabled, these can be determined by calling \p bsx_update_subscription with \p *n_physical_sensor_config_p
 *          set to zero.
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_DOSTEPS_INVALIDINPUT
 *  @retval BSX_E_DOSTEPS_DUPLICATEINPUT
 *  @retval BSX_E_DOSTEPS_TSINTERDIFFOUTOFRANGE
 *  @retval BSX_E_DOSTEPS_TSINTRADIFFOUTOFRANGE
 *  @retval BSX_E_DOSTEPS_VALUELIMITS
 *
 * @note The parameter \p threads tells from which thread bsx_do_steps is called to avoid having to many interfaces to BSX.
 *       Currently supported values for \p threads are at the moment only 0x01U or 0xFFU, i.e. usage of BSX from different threads
 *       in a multi-thread environment is yet not supported.
 *
 * @note Usage and evaluation of the sensor identifier is mandatory for \p data_in_p and \p data_out_p. The order
 *       of sensor signals for library inputs as well outputs, i.e. \p data_in_p and \p data_out_p, is arbitrary,
 *       may and can be changed from function call to function call and depends on the library solution.
 *
 * @note If the number of outputs to be returned by \p data_out_p is greater than the provided maximum length in
 *       \p n_data_out_p, a warning is returned and the remaining outputs can be read by calling bsx_get_output_signal
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesDosteps "processing of signals".
 */
bsx_return_t bsx_do_steps(bsx_instance_t * const bsx_p, const bsx_u8_t threads,
                          bsx_fifo_data_t *data_in_p, const bsx_u32_t n_data_in,
                          bsx_fifo_data_t *data_out_p, bsx_u32_t *n_data_out_p);


/*! @brief Retrieve the sample of the given single sensor of the library
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[out]      data_out_p                      Sample of the virtual sensor representing a library output
 * @param[in]       sensor_id                       Identifier of a virtual sensor representing a library output
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_GET_DISABLED
 *  @retval BSX_E_GET_NODATA
 *  @retval BSX_E_INVALIDSTATE
 *
 * @note On calling the function, provide the buffer for a frame set and the buffer size by the maximum number of dimensions for a frame
 *       and the number of frames in a set.
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesGetoutputsignal "retrieving output signals".
 */
bsx_return_t bsx_get_output_signal(bsx_instance_t * const bsx_p, bsx_fifo_data_t * data_out_p,
                                   const bsx_s16_t sensor_id);

/*! @brief Update the subscription of the library client to provided virtual sensors
 *
 * Returns the required settings for the physical sensors that are providing the input data. The settings returned by this function
 * have to be applied to the physical sensors. The function supports two modes:
 *  -# change the subscription and return actual configuration of virtual sensors as well as required configuration for physical sensors.
 *     Provide enough entries using \p physical_sensor_config_p and \p n_physical_sensor_config_p.
 *  -# tell the current subscription by returning the actual configuration of all virtual sensors. Pass zero for n_physical_sensor_config_p
 *     and provide large enough number of entries using \p virtual_sensor_config_p and \p n_virtual_sensor_config_p
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in,out]   virtual_sensor_config_p         On calling the function: array of configurations for virtual sensors representing library outputs;
 *                                                  On function return: current configuration for requested outputs depending on library restrictions as well as other provided outputs
 * @param[in,out]   n_virtual_sensor_config_p       On calling the function: provided length of \p virtual_sensor_config_p;
 *                                                  On function return: number of modified values in \p virtual_sensor_config_p
 * @param[in,out]   physical_sensor_config_p        Array of settings for physical sensors providing the library inputs depending. Content is not
 *                                                  changed when \p n_physical_sensor_config_p is zero.
 * @param[in,out]   n_physical_sensor_config_p      On calling the function: Zero to return the current library configuration or maximum length of \p physical_sensor_config_p;
 *                                                  On function return: Number of elements changed in \p physical_sensor_config_p
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_I_SU_SUBSCRIBEDOUTPUTGATES
 *  @retval BSX_W_SU_NOOUTPUTGATE
 *  @retval BSX_W_SU_UNKNOWNOUTPUTGATE
 *  @retval BSX_E_SU_GATECOUNTEXCEEDSARRAY
 *  @retval BSX_E_SU_DUPLICATEGATE
 *  @retval BSX_E_SU_SAMPLERATELIMITS
 *  @retval BSX_E_SU_INVALIDSAMPLERATE
 *  @retval BSX_E_SU_ITERATIONSEXCEEDED
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_FATAL
 *
 * @note Usage and evaluation of the sensor identifier is mandatory. The order of configuration settings for library inputs
 *       as well outputs, i.e. \p physical_sensor_config_p and \p virtual_sensor_config_p, is arbitrary, may and can be
 *       changed from function call to function call and depends on the library solution.
 * @note If \p n_physical_sensor_config_p is set to zero,
 *         - the current subscription to library outputs is returned within \p virtual_sensor_config_p
 *         - the maximum length of \p virtual_sensor_config_p provided by \p n_virtual_sensor_config_p must be at least the number of provided virtual sensors
 *         - the subscription to library outputs is not changed regardless of the configuration written into \p virtual_sensor_config_p.
 *
 * Further information and example sequences for updating the subscription are available in the integration guideline for \ref intguideInterfacesUpdatesubscription "the subscription update functionality".
 */
bsx_return_t bsx_update_subscription(bsx_instance_t *const bsx_p,
                                     bsx_sensor_configuration_t *const virtual_sensor_config_p,
                                     bsx_u32_t *const n_virtual_sensor_config_p,
                                     bsx_sensor_configuration_t *const physical_sensor_config_p,
                                     bsx_u32_t *const n_physical_sensor_config_p);


/*! @brief Update the current configuration of the library
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       serialized_settings_p           Serialized configuration settings
 * @param[in]       n_serialized_settings           Size of the configuration settings serialization
 * @param[in,out]   work_buffer_p                   Work buffer used to apply the serialization
 * @param[in]       n_work_buffer                   Size of the provided work buffer. Must be at least 512 bytes.
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_CONFIG_EMPTY
 *  @retval BSX_E_CONFIG_VERSIONMISMATCH
 *  @retval BSX_E_CONFIG_CRCMISMATCH
 *  @retval BSX_E_CONFIG_FEATUREMISMATCH
 *  @retval BSX_E_CONFIG_INVALIDSECTIONCOUNT
 *  @retval BSX_E_CONFIG_INSUFFICIENTBUFFER
 *  @retval BSX_E_CONFIG_INSUFFICIENTWORKBUFFER
 *  @retval BSX_E_PARSE_SECTIONEXCEEDSWORKBUFFER
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_FATAL
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesSetconfiguration "update of the BSX configuration".
 */
bsx_return_t bsx_set_configuration(bsx_instance_t * const bsx_p,
                                   bsx_u8_t *serialized_settings_p, const bsx_u32_t n_serialized_settings,
                                   bsx_u8_t *work_buffer_p, const bsx_u32_t n_work_buffer);

/*! @brief Retrieve the current configuration of the library for the given set identifier
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       config_set_id                   Identifier for a specific set of configuration settings to be returned.
 * @param[in,out]   serialized_configuration_p      Configuration settings serialized
 * @param[in]       n_serialized_configuration_max  Size of the buffer to store the serialization
 * @param[in,out]   work_buffer_p                   Work buffer used to build the serializations
 * @param[in]       n_work_buffer                   Size of the provided work buffer. Must be at least 512 bytes.
 * @param[out]      n_serialized_configuration_p    Size of the returned serialization of configuration settings
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_CONFIG_INSUFFICIENTBUFFER
 *  @retval BSX_E_CONFIG_INSUFFICIENTWORKBUFFER
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_FATAL
 *
 * @note The configuration shall be zero when to retrieve all configuration settings. Please check the table of identifiers in
 *       @ref intguideQuickintroConfigState "update and retrieval of configuration and state".
 * @note Beware that the size of the serialization can be huge when trying to retrieve the complete
 *       set depending on the features provided by the library solution.
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesGetconfiguration "retrieval of the BSX configuration".
 */
bsx_return_t bsx_get_configuration(bsx_instance_t * const bsx_p, const bsx_u8_t config_set_id,
                                   bsx_u8_t *serialized_configuration_p, const bsx_u32_t n_serialized_configuration_max,
                                   bsx_u8_t *work_buffer_p, const bsx_u32_t n_work_buffer,
                                   bsx_u32_t *n_serialized_configuration_p);

/*! @brief Update the state of the library instance partially or complete depending on the passed serialization
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       serialized_state_p              Serialized states
 * @param[in]       n_serialized_state              Size of the state serialization
 * @param[in,out]   work_buffer_p                   Work buffer used to apply the serialization
 * @param[in]       n_work_buffer                   Size of the provided work buffer. Must be at least 512 bytes.
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_CONFIG_EMPTY
 *  @retval BSX_E_CONFIG_VERSIONMISMATCH
 *  @retval BSX_E_CONFIG_CRCMISMATCH
 *  @retval BSX_E_CONFIG_FEATUREMISMATCH
 *  @retval BSX_E_CONFIG_INVALIDSECTIONCOUNT
 *  @retval BSX_E_CONFIG_INSUFFICIENTBUFFER
 *  @retval BSX_E_CONFIG_INSUFFICIENTWORKBUFFER
 *  @retval BSX_E_PARSE_SECTIONEXCEEDSWORKBUFFER
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_FATAL
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesSetstate "update of the BSX state".
 */
bsx_return_t bsx_set_state(bsx_instance_t * const bsx_p,
                           bsx_u8_t *serialized_state_p, const bsx_u32_t n_serialized_state,
                           bsx_u8_t *work_buffer_p, const bsx_u32_t n_work_buffer);


/*! @brief Retrieve the current state of the library for the given set identifier
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       state_set_id                    Identifier for a specific set of states to be returned.
 * @param[in,out]   serialized_state_p              Serialized states serialized to a string
 * @param[in]       n_serialized_state_max          Maximum available size for the serialized states
 * @param[in,out]   work_buffer_p                   Work buffer used to build the serializations
 * @param[in]       n_work_buffer                   Size of the provided work buffer. Must be at least 512 bytes.
 * @param[out]      n_serialized_state_p            Size of the returned serialization of states
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_CONFIG_INSUFFICIENTBUFFER
 *  @retval BSX_E_CONFIG_INSUFFICIENTWORKBUFFER
 *  @retval BSX_E_INVALIDSTATE
 *  @retval BSX_E_FATAL
 *
 * @note The set identifier shall be zero when to retrieve all states. Please check the table of identifiers in
 *       @ref intguideQuickintroConfigState "update and retrieval of configuration and state".
 * @note Beware that the size of the serialization can be huge when trying to retrieve the complete
 *       set depending on the features provided by the library solution.
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesGetstate "retrieval of the BSX state".
 */
bsx_return_t bsx_get_state(bsx_instance_t * const bsx_p, const bsx_u8_t state_set_id,
                           bsx_u8_t *serialized_state_p, const bsx_u32_t n_serialized_state_max,
                           bsx_u8_t *work_buffer_p, const bsx_u32_t n_work_buffer,
                           bsx_u32_t *n_serialized_state_p);

/*! @brief Reset all configuration settings and states to defaults
 *
 * @param[in,out]   bsx_p                           Instance of the library
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *
 * @note A configuration of the library instance is required to allow signal-processing.
 *
 * Further information is available in the integration guideline for \ref intguideInterfacesReset "reset of BSX".
 */
bsx_return_t bsx_reset(bsx_instance_t * const bsx_p);

/*! @brief Set the debug level of the library instance that determines which information can be dumped
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in]       debug_level                     Bit map of debugging levels
 * @param[in]       observation_p                   Additional information required to support evaluation for certain debug levels
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *
 * @note Re-configuration of the library might be required to enable certain debug levels.
 */
bsx_return_t bsx_set_debug_level(bsx_instance_t *const bsx_p, const bsx_u32_t debug_level, void *const observation_p);

/*! @brief Dump internal values of the library instance for analysis and debugging purposes
 *
 * @param[in,out]   bsx_p                           Instance of the library
 * @param[in,out]   output_p                        Pointer to a memory space to store the debug information to be returned
 * @param[in]       n_output                        Maximum size of the provided memory space
 * @param[out]      n_output_p                      Size of the returned debug information
 *
 * @return Zero when successful, positive value for information and warnings, otherwise a negative value as error code.
 *  @retval BSX_OK
 *  @retval BSX_E_INVALIDSTATE
 *
 * @note A call to bsx_set_debug_level is required before useful information can be returned by this function.
 */
bsx_return_t bsx_dump(bsx_instance_t * const bsx_p, bsx_u8_t *const output_p,
                      const bsx_u32_t n_output, bsx_u32_t * const n_output_p);
#ifdef __cplusplus
}
#endif

#endif /*__BSX_LIBRARY_H__*/
/*! @}*/

/*
 * Copyright (c) quaming
 *
 * Header file for UMS (USB Mass Storage) related functions and callbacks.
 */

#ifndef __QM_UMS_H__
#define __QM_UMS_H__

/**
 * @brief Callback type for mass storage operations.
 *
 * This callback is invoked during mass storage operations.
 * @return 0 on success, or a negative error code on failure.
 */
typedef int (*qm_mass_storage_callback)(void);

/**
 * @brief Callback type for PC connection events.
 *
 * This callback is invoked when the PC connection state changes.
 * @return 0 on success, or a negative error code on failure.
 */
typedef int (*qm_pc_connect_callback)(void);

/**
 * @brief Get the UDC (USB Device Controller) name.
 *
 * @param udc_name A buffer to store the UDC name.
 * @param size The size of the buffer.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gudc_name(char *udc_name, size_t size);

/**
 * @brief Disable mass storage functionality.
 */
void qm_mass_storage_disable(void);

/**
 * @brief Enable mass storage functionality.
 *
 * @param file_path The path to the file or partition to be used for mass
 * storage.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_mass_storage_enable(const char *file_path);

/**
 * @brief Check if mass storage is enabled, with a callback to notify.
 *
 * @return 0 if mass storage is enabled, or a negative error code if disabled.
 */
int qm_check_mass_storage_enabled_cb(void);

/**
 * @brief Check if the PC connection is enabled, with a callback to notify.
 *
 * @return 0 if PC connection is enabled, or a negative error code if disabled.
 */
int qm_check_pc_connect_enabled_cb(void);

#endif

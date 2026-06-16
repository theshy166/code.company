/*
 * Copyright (c) quaming
 *
 * Header file for system-level functions, including chip information retrieval,
 * system power management, and device identification.
 */

#ifndef __QM_SYSTEM_H__
#define __QM_SYSTEM_H__

/**
 * @brief Enum representing the different suspend types.
 */
typedef enum {
  SUSPEND_FREEZE = 0, /**< Freeze system (low-power state) */
  SUSPEND_MEM,        /**< Suspend to memory (sleep mode) */
} SUSPEND_TYPE;

/**
 * @brief Get the chip ID.
 *
 * @param chipid A buffer to store the chip ID (must be large enough to hold the
 * chip ID).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_chip_id_get(char *chipid);

/**
 * @brief Read data from a vendor-specific register or memory.
 *
 * @param vendor_id The vendor ID to access the specific data.
 * @param data The buffer to store the read data.
 * @param size The size of the data buffer.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_vendor_read(int vendor_id, char *data, int size);

/**
 * @brief Write data to a vendor-specific register or memory.
 *
 * @param vendor_id The vendor ID to access for writing data.
 * @param data The data to write.
 * @param size The size of the data to write.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_vendor_write(int vendor_id, const char *data, int size);

/**
 * @brief Reboot the system.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_reboot(void);

/**
 * @brief Suspend the system to the specified power state.
 *
 * @param type The suspend type (freeze or memory suspend).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_suspend(SUSPEND_TYPE type);

/**
 * @brief Shutdown the system.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_shutdown(void);

/**
 * @brief Get the hardware model information.
 *
 * @param pdata A pointer to a buffer where the hardware model information will
 * be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_hardware_model_get(void *pdata);

/**
 * @brief Get the product model information.
 *
 * @param pdata A pointer to a buffer where the product model information will
 * be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_product_model_get(void *pdata);

/**
 * @brief Get the serial ID of the device.
 *
 * @param pdata A pointer to a buffer where the serial ID will be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_serial_id_get(void *pdata);

/**
 * @brief Get the device type information.
 *
 * @param pdata A pointer to a buffer where the device type will be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_device_type_get(void *pdata);

/**
 * @brief Get the manufacturer information.
 *
 * @param pdata A pointer to a buffer where the manufacturer information will be
 * stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_manufacture_get(void *pdata);

#endif

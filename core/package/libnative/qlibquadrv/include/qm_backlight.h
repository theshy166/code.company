/*
 * Copyright (c) Quaming
 *
 * This header file defines the interface for controlling the backlight
 * brightness. It includes functions to set and get the brightness level of the
 * backlight.
 */
#ifndef QM_BACKLIGHT_H
#define QM_BACKLIGHT_H
/**
 * @brief Set the brightness level of the backlight.
 *
 * This function adjusts the brightness of the backlight to the specified level.
 *
 * @param brightness The desired brightness level, typically within a range
 * defined by the hardware.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_backlight_setbri(uint32_t brightness);

/**
 * @brief Get the current brightness level of the backlight.
 *
 * This function retrieves the current brightness level of the backlight.
 *
 * @return The current brightness level on success, or a negative error code on
 * failure.
 */
int qm_backlight_getbri(void);

#endif // QM_BACKLIGHT_H

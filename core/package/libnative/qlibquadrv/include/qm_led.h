/*
 * Copyright (c) quaming
 *
 * Header file for LED control interface.
 * Provides functionality to set LED mode (e.g., blink, brightness).
 */
#ifndef QM_LED_H
#define QM_LED_H

/**
 * @brief Set the LED mode.
 *
 * @param device_path Path to the LED device.
 * @param blink Whether to blink the LED (true for blink, false for static).
 * @param brightness LED brightness value (0 to 100).
 * @return 0 on success, negative error code on failure.
 */
int qm_led_set_mode(char *device_path, bool blink, uint32_t brightness);

#endif

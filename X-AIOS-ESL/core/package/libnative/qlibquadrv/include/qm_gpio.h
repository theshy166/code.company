/*
 * Copyright (c) Quaming
 *
 * This header file defines the interface for GPIO (General-Purpose
 * Input/Output) control. It includes functions to export/unexport GPIO pins,
 * set/get their direction, and read/write values.
 */
#ifndef QM_GPIO_H
#define QM_GPIO_H
/**
 * @brief Export a GPIO pin for use.
 *
 * This function exports the specified GPIO pin, making it available for use.
 *
 * @param gpio The GPIO pin number to export.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gpio_export(uint32_t gpio);

/**
 * @brief Unexport a GPIO pin.
 *
 * This function unexports the specified GPIO pin, releasing it for other uses.
 *
 * @param gpio The GPIO pin number to unexport.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gpio_unexport(uint32_t gpio);

/**
 * @brief Set the direction of a GPIO pin.
 *
 * This function sets the direction of the specified GPIO pin as either input or
 * output.
 *
 * @param gpio The GPIO pin number.
 * @param input Set to true to configure the pin as input, false for output.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gpio_set_direction(uint32_t gpio, bool input);

/**
 * @brief Get the direction of a GPIO pin.
 *
 * This function retrieves the direction of the specified GPIO pin.
 *
 * @param gpio The GPIO pin number.
 * @return 0 if output, 1 if input, or a negative error code on failure.
 */
int qm_gpio_get_direction(uint32_t gpio);

/**
 * @brief Export a GPIO pin and set its direction.
 *
 * This function exports the specified GPIO pin and sets its direction.
 *
 * @param gpio The GPIO pin number.
 * @param input Set to true to configure the pin as input, false for output.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gpio_export_direction(uint32_t gpio, bool input);

/**
 * @brief Set the value of a GPIO pin.
 *
 * This function sets the output value of the specified GPIO pin.
 *
 * @param gpio The GPIO pin number.
 * @param value The value to set (typically 0 or 1).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_gpio_set_value(uint32_t gpio, int value);

/**
 * @brief Get the value of a GPIO pin.
 *
 * This function retrieves the input value of the specified GPIO pin.
 *
 * @param gpio The GPIO pin number.
 * @return The value of the GPIO pin (typically 0 or 1), or a negative error
 * code on failure.
 */
int qm_gpio_get_value(uint32_t gpio);

#endif // QM_GPIO_H

/*
 * Copyright (c) quaming
 *
 * Header file for Pulse Width Modulation (PWM) control.
 * Provides functions to configure PWM settings, such as period, duty cycle,
 * polarity, and enable state.
 */

#ifndef __QM_PWM_H__
#define __QM_PWM_H__

/**
 * @brief Enum for PWM polarity settings.
 */
enum pwm_polarity {
  PWM_POLARITY_NORMAL,   /**< Normal polarity (default) */
  PWM_POLARITY_INVERSED, /**< Inverted polarity */
};

/**
 * @brief Export a PWM pin for use.
 *
 * @param pwm The PWM pin number to export.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_export(uint32_t pwm);

/**
 * @brief Unexport a PWM pin.
 *
 * @param pwm The PWM pin number to unexport.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_unexport(uint32_t pwm);

/**
 * @brief Set the period for a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @param period The PWM period in microseconds.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_set_period(uint32_t pwm, uint32_t period);

/**
 * @brief Get the current period of a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @return The PWM period in microseconds, or a negative error code on failure.
 */
int qm_pwm_get_period(uint32_t pwm);

/**
 * @brief Set the duty cycle for a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @param duty The duty cycle as a percentage (0 to 100).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_set_duty(uint32_t pwm, uint32_t duty);

/**
 * @brief Get the current duty cycle of a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @return The duty cycle as a percentage (0 to 100), or a negative error code
 * on failure.
 */
int qm_pwm_get_duty(uint32_t pwm);

/**
 * @brief Set the polarity of a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @param polarity The desired polarity (normal or inverted).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_set_polarity(uint32_t pwm, enum pwm_polarity polarity);

/**
 * @brief Get the current polarity of a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @return The current polarity (normal or inverted), or a negative error code
 * on failure.
 */
int qm_pwm_get_polarity(uint32_t pwm);

/**
 * @brief Enable or disable a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @param enabled Boolean indicating whether to enable (true) or disable (false)
 * the PWM.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_set_enable(uint32_t pwm, bool enabled);

/**
 * @brief Get the current enable state of a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @return 1 if enabled, 0 if disabled, or a negative error code on failure.
 */
int qm_pwm_get_enable(uint32_t pwm);

/**
 * @brief Initialize a PWM signal with the specified parameters.
 *
 * @param pwm The PWM pin number.
 * @param period The PWM period in microseconds.
 * @param duty The duty cycle as a percentage (0 to 100).
 * @param polarity The desired polarity (normal or inverted).
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_init(uint32_t pwm, uint32_t period, uint32_t duty,
                enum pwm_polarity polarity);

/**
 * @brief Deinitialize a PWM signal.
 *
 * @param pwm The PWM pin number.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_pwm_deinit(uint32_t pwm);

#endif

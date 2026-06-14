/*
 * Copyright (c) quaming
 *
 * Header file for system time management, including getting and setting the
 * system time, alarm management, and waiting for alarms.
 */
#ifndef __QM_TIME_H__
#define __QM_TIME_H__
/**
 * @brief Get the current system time.
 *
 * @param time Pointer to a `struct tm` where the current time will be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_get_time(struct tm *time);

/**
 * @brief Set the system time.
 *
 * @param time Pointer to a `struct tm` containing the time to set.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_set_time(struct tm *time);

/**
 * @brief Get the current alarm time.
 *
 * @param time Pointer to a `struct tm` where the alarm time will be stored.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_get_alarm(struct tm *time);

/**
 * @brief Set the alarm time.
 *
 * @param time Pointer to a `struct tm` containing the alarm time to set.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_set_alarm(struct tm *time);

/**
 * @brief Enable the system alarm.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_enable_alarm(void);

/**
 * @brief Disable the system alarm.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_system_disable_alarm(void);

/**
 * @brief Wait for the alarm to trigger, with a timeout.
 *
 * @param timeout The maximum time to wait in milliseconds.
 * @return 0 if the alarm triggered, or a negative error code on timeout or
 * failure.
 */
int qm_system_wait_alarm(uint32_t timeout);

#endif

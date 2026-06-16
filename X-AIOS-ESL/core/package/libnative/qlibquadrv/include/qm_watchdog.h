/*
 * Copyright (c) quaming
 *
 * Header file for Watchdog timer functions.
 * Provides functionality to start, stop, and refresh the watchdog timer.
 */

#ifndef __QM_WATCHDOG_H__
#define __QM_WATCHDOG_H__

/**
 * @brief Start the watchdog timer.
 *
 * This function starts the watchdog timer with a specified timeout.
 * If the timer is not refreshed before the timeout, the system may reset.
 *
 * @param timeval Timeout value in seconds for the watchdog timer.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_watchdog_start(int timeval);

/**
 * @brief Stop the watchdog timer.
 *
 * This function stops the watchdog timer, preventing any future resets from
 * occurring.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_watchdog_stop(void);

/**
 * @brief Refresh the watchdog timer.
 *
 * This function refreshes the watchdog timer to prevent a reset.
 * It should be called periodically before the timeout expires.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_watchdog_refresh(void);

#endif //__QM_WATCHDOG_H__

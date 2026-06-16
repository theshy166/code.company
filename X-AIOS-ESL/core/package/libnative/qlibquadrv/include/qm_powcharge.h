/*
 * Copyright (c) quaming
 *
 * Header file for power charging related functions.
 * Provides functions to query battery status, charging state, and manage
 * battery percentages.
 */
#ifndef QM_POWCHARGE_H
#define QM_POWCHARGE_H

/**
 * @brief Get the current battery percentage.
 *
 * @return The current battery percentage as an integer (0-100).
 */
int qm_get_battery_percentage(void);

/**
 * @brief Get the current charging status.
 *
 * @return 1 if charging, 0 if not charging.
 */
int qm_get_charging_status(void);

/**
 * @brief Set the initial battery percentage during startup.
 *
 * @param stored_percentage The battery percentage to set at startup.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_startup_battery_percentage(int stored_percentage);

/**
 * @brief Check if the battery is full.
 *
 * @return True if the battery is full, false otherwise.
 */
bool qm_is_battery_full(void);

#endif

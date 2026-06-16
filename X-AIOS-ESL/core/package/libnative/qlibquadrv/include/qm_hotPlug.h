/*
 * Copyright (c) quaming
 *
 * Header file for hot plug functionality.
 * Provides functions for managing USB hot plug events and registering
 * callbacks.
 */
#ifndef QM_HOT_PLUG_H
#define QM_HOT_PLUG_H


/** 
 * @brief Enum representing the possible actions for USB hot plug events.
 */
typedef enum {
    HOTPLUG_ACTION_NO = 0,   /**< No action */
    HOTPLUG_ACTION_ADD,      /**< Device added */
    HOTPLUG_ACTION_REMOVE   /**< Device removed */
} HOTPLUG_ACTION;

/**
 * @brief Callback type for hot plug events.
 * This function is called when a USB device is inserted or removed.
 *
 * @param action The action that occurred (insert/remove).
 * @param device The device name or identifier associated with the event.
 */
typedef void (*HotPlugCallback)(HOTPLUG_ACTION action, const char *device);

/**
 * @brief Stop the hot plug event listener.
 *
 * This function stops monitoring for hot plug events and unregisters the
 * callback.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_hotplug_stop(void);

/**
 * @brief Register a callback for USB hot plug events.
 *
 * This function registers a callback function that will be invoked when a USB
 * device is inserted or removed.
 *
 * @param callback The callback function to be called on hot plug events.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_usb_hotplug(HotPlugCallback callback);

#endif

/*
 * Copyright (c) Quaming
 *
 * This header file defines the interface for event handling, including
 * functions to register and unregister event devices, as well as start and stop
 * event listening.
 */
#ifndef QM_EVENT_H
#define QM_EVENT_H
/**
 * @brief Type definition for event handler callback function.
 *
 * This function is called when an input event is received.
 *
 * @param event Pointer to the input event structure.
 */
typedef void (*event_handler_t)(struct input_event *);

/**
 * @brief Register an event device.
 *
 * This function registers an event device specified by the device path.
 *
 * @param dev_path Path to the event device.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_event_register(char *dev_path);

/**
 * @brief Unregister an event device.
 *
 * This function unregisters an event device specified by the device path.
 *
 * @param dev_path Path to the event device.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_event_unregister(char *dev_path);

/**
 * @brief Start listening for events.
 *
 * This function starts the event listening process and registers a handler to
 * be called when events are received.
 *
 * @param handler Function pointer to the event handler.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_event_listen_start(event_handler_t handler);

/**
 * @brief Stop listening for events.
 *
 * This function stops the event listening process.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int qm_event_listen_stop(void);

#endif // QM_EVENT_H

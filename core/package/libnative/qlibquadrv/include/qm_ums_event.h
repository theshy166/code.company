/*
 * Copyright (c) 2022  quaming
 *
 */
#ifndef __QM_UMS_EVENT_H__
#define __QM_UMS_EVENT_H__

// pc connect state
typedef enum {
	QM_PC_CONNECT_STATE_DISCONNECTED = 0,
	QM_PC_CONNECT_STATE_CONNECTED,
	QM_PC_CONNECT_STATE_UNKNOWN = 255,
}QM_PC_CONNECT_STATE;

// pc connect event callback
typedef void (*qm_pc_connect_event_callback)(QM_PC_CONNECT_STATE state);

/*
 * register pc connect event callback
 * return 0 if success, -1 if failed
 */
int qm_register_pc_connect_event_callback(qm_pc_connect_event_callback callback);
/*
 * unregister pc connect event callback
 * return 0 if success, -1 if failed
 */
int qm_unregister_pc_connect_event_callback(qm_pc_connect_event_callback callback);
#endif
#define LOS_USE_MQTT
#ifdef LOS_USE_MQTT
#ifndef LOS_DOOR_LOCK_H
#define LOS_DOOR_LOCK_H

#ifdef __cplusplus
extern "C" {
#endif

char* parse_lock_switch_json(char* json_string);





#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LOS_DOOR_LOCK_H*/
#endif
#ifndef __ESL_SIGN_H__
#define __ESL_SIGN_H__


char *generate_sign(const char *deviceno, const char *type);

char *generate_push_server_sign(const char *deviceno, const char *type, const char *protocol_type, const char *local_ip,const char *version_no);

char *generate_check_life_sign(const char *deviceno, const char *type, const char *device_list, const char *time);

char *generate_push_message_report(const char *deviceno, const char *type, const char *push_id, const char *push_type,const char *time);

#endif // __ESL_SIGN_H__
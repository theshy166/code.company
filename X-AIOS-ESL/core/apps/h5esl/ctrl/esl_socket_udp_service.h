#ifndef __ESL_SOCKET_UDP_SERVICE_H__
#define __ESL_SOCKET_UDP_SERVICE_H__

void send_broadcast_message(char * broadcast_ip,char * msg);

void recevice_msg_and_send(int group_id_1,int group_id_2);

void set_local_group_id_1(int group_id);
void set_local_group_id_2(int group_id);

int get_local_group_id(int index);

#endif // __ESL_SOCKET_UDP_SERVICE_H__
#ifndef _SYSCMD_API_H_
#define _SYSCMD_API_H_

void *qm_sys_cmd_open(int cmdId);
char *qm_sys_cmd_read(void *handle, char *buffer, size_t buffer_size);
char *qm_sys_cmd_close(void *handle);
int  qm_once_syscmd_read(int cmdId, char buffer, int length);

#endif

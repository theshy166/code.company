#ifndef _SERIAL_NUMBER_H
#define _SERIAL_NUMBER_H

extern char* deviceSerialNumber;

extern char* clientid;

void initializeSerialNumber(void);
void cleanupSerialNumber(void);  // 添加清理函数声明

#endif
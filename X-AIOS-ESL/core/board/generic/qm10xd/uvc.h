// Authtor: dengxiaomin
// Revision History: V1.0-2019/03/29

#ifndef _UVC_CLIENT_H
#define _UVC_CLIENT_H

#include <stdio.h>

int uvcOpen(int chn);
int uvcClose(int chn);
int uvcRead(int chn, unsigned char** pBuf);
int uvcRelease(int chn);

#endif

#ifndef _APPMESSAGE_CODE_ENUM_H
#define _APPMESSAGE_CODE_ENUM_H

#include <stdio.h>
// 定义一个枚举类型
enum APP_MESSAGE_CODE_ENUM {
    POST_MIXED_PLAY_CODE = 1,//mixed play
    POST_SHOW_OR_HIDDEN_CODE = 2,//videoplay or imageviewer send message if they are show or hidden.
    POST_SYNC_FILE_LIST_CODE = 3,
    POST_MEDIA_PLAYER_SHOW_FILE = 4,
    POST_MEDIA_PLAYER_DATA_CHANGED = 5,	
    POST_RELOAD_FILES = 6,	
    BROADCAST_SETTING_PAD_CLOSE_CODE = 999,//settingpad 关闭时发送的消息
    BROADCAST_SHUTDOWN_START = 1000,
};

#endif //_APPMESSAGE_CODE_ENUM_H

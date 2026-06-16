/*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
* 
*/

#ifndef _APP_INTENT_H
#define _APP_INTENT_H
/*
 * struct Intent
 * it is used to transfer data between applications.
 */
typedef struct _Intent {
    int param0;
    int param1;
    int param2;
    void * user_data;       /* extended user data*/
}Intent;

#endif  /*_APP_INTENT_H*/

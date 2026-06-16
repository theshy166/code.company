 #if defined (CONFIG_XOS_FWK_CADP_KUQI)  && !defined(BUILD_SIMULATOR)
#ifndef LOS_KUQI_ENTRY__H
#define LOS_KUQI_ENTRY__H

#include "cadp_aiskill.h"
typedef struct {  
	char appkey[20];
	char secretkey[30];
	char deviceid[20];
	char recordpath[20];
    char image_path[100];	
	int  server_type;
	int  image_ready;	
	int  state_coapi;
	int  reg;
	int  coapi_link_status;
	int  toStop;
	unsigned char  boot_completed;
	void (*respose)(const AiMessage* params); 
    void (*connectCallback)(const AiMessage* params); 
	pthread_mutex_t mutex;
	pthread_cond_t cond ;
} coapi_struct;


#endif/*LOS_KUQI_ENTRY__H*/
#endif/*CONFIG_XOS_FWK_CADP_KUQI*/
#if 1//def CONFIG_XOS_FWK_CADP_SAMPLE
#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <string.h>
#include "cadp_aiskill.h"
#include "cadp_aiskill_template.h"

void sample_ai_init( AiDevInfo* devinfo ){   
}

void sample_ai_deinit( ){
}

int sample_register_respose_message(respose callback){

}


int sample_register_connect_callback(connectCallback callback){

}

int sample_send_cmd(AiMessage* msg){

    return 0;
}

int sample_upload_file(AiMessage* msg){
     return 0;
}

AiModule g_SampleAiMoudule = {
    .init = sample_ai_init,
    .deinit = sample_ai_deinit,
    .register_respose = sample_register_respose_message,
    .register_conn_respose = sample_register_connect_callback,
    /****/
    .send_cmd = sample_send_cmd,
    .input_content = sample_upload_file
};

#endif/*CONFIG_XOS_FWK_CADP_SAMPLE*/
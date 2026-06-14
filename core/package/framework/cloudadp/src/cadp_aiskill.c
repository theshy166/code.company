#include <stdio.h>  
#include <stdlib.h>  
#include "cadp_aiskill.h"


extern  AiModule g_SampleAiMoudule;
#if defined(CONFIG_XOS_FWK_CADP_QUA)
extern  AiModule g_QuaCMoudule;
#endif


AiModule* create_module(AiModuleType type) {  
    switch(type) {  
        case MODULE_DEMO:  
            return &g_SampleAiMoudule;  
 #if defined(CONFIG_XOS_FWK_CADP_QUA)
        case MODULE_QUA:  
            return &g_QuaCMoudule;
 #endif               

        default:  
            return NULL;  
    }  
}


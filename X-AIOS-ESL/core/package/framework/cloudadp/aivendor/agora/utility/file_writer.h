#ifndef __AGORA_FILE_WRITER_H__
#define __AGORA_FILE_WRITER_H__

#include "vc_ability.h"

#ifdef CONFIG_XOS_FWK_CADP_AGORA

enum {
  FILE_TYPE_AUDIO = 1,
  FILE_TYPE_VIDEO = 2,
};

void *create_file_writer(uint8_t file_type, const char *base_name);
int write_file(void *file_writer, uint8_t data_type, const void *data, size_t size);
void destroy_file_writer(void * file_writer);

#endif //#ifdef CONFIG_XOS_FWK_CADP_AGORA

#endif

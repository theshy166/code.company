/*************************************************************
 * Author:	wangjiangyuan (wangjiangyuan@agora.io)
 * Date	 :	Oct 21th, 2020
 * Module:	Agora SD-RTN SDK RTC C API demo application.
 *
 *
 * This is a part of the Agora RTC Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#include <string.h>

#include "file_parser.h"
#include "file_parser_priv.h"

#ifdef CONFIG_XOS_FWK_CADP_AGORA

#define SUPPORT_JPEG    0
#define SUPPORT_YUV420  0
#define SUPPORT_G711    0
#define SUPPORT_G722    0
#define SUPPORT_AAC     0
#define SUPPORT_OPUS    0
#define SUPPORT_H265    0

extern media_parser_t video_parser_h264;
#if SUPPORT_JPEG
extern media_parser_t video_parser_jpeg;
#endif
#if SUPPORT_YUV420
extern media_parser_t video_parser_yuv420;
#endif
#if SUPPORT_H265
extern media_parser_t video_parser_h265;
#endif
#if SUPPORT_OPUS
extern media_parser_t audio_parser_opus;
#endif
#if SUPPORT_AAC
extern media_parser_t audio_parser_aac;
#endif
extern media_parser_t audio_parser_pcm;
#if SUPPORT_G711
extern media_parser_t audio_parser_g711;
#endif
#if SUPPORT_G722
extern media_parser_t audio_parser_g722;
#endif

static media_parser_t *gs_media_parser_tab[] = {
                              &video_parser_h264,
                            #if SUPPORT_JPEG
                              &video_parser_jpeg,
                            #endif
                            #if SUPPORT_H265
                              &video_parser_h265,
                            #endif
                            #if SUPPORT_YUV420
                              &video_parser_yuv420,
                            #endif
                            #if SUPPORT_AAC
                              &audio_parser_aac,
                            #endif
                              &audio_parser_pcm,
                            #if SUPPORT_G711
                              &audio_parser_g711,
                            #endif
                            #if SUPPORT_G722
                              &audio_parser_g722,
                            #endif
                            #if SUPPORT_OPUS
                              &audio_parser_opus,
                            #endif
                            };
static int gs_media_parser_cnt = sizeof(gs_media_parser_tab) / sizeof(gs_media_parser_tab[0]);

void *create_file_parser(media_file_type_e type, const char *path, parser_cfg_t *p_parser_cfg)
{
  int i;
  for (i = 0; i < gs_media_parser_cnt; i++) {
    if (type == gs_media_parser_tab[i]->codec) {
      break;
    }
  }

  if (i == gs_media_parser_cnt) {
    // type not found, return NULL
    AGO_LOGE("Can't find the file parser matching type %d", type);
    return NULL;
  }

  AGO_LOGI("File parser found: %s", gs_media_parser_tab[i]->name);

  // type found, now create the File parser
  media_parser_t *parser = (media_parser_t *)malloc(sizeof(media_parser_t));
  if (parser == NULL) {
    return NULL;
  }
  memcpy(parser, gs_media_parser_tab[i], sizeof(media_parser_t));
  if (p_parser_cfg) {
    memcpy(&(parser->parser_cfg), p_parser_cfg, sizeof(parser_cfg_t));
  }

  if (parser->open(parser, path) < 0) {
    free(parser);
    AGO_LOGE("File parser can't open file %s", path);
    return NULL;
  }

  return (void *)parser;
}

void destroy_file_parser(void *p_parser)
{
  if (p_parser) {
    media_parser_t *parser = (media_parser_t *)p_parser;
    parser->close(parser);
    free(parser);
  }
}

int file_parser_obtain_frame(void *p_parser, frame_t *p_frame)
{
  int ret;

  if (!p_parser) {
    return -1;
  }

  media_parser_t *parser = (media_parser_t *)p_parser;

  ret = parser->obtain_frame(parser, p_frame);
  if (ret == -2) {
    parser->reset(parser);
    AGO_LOGI("File parser has reached the end of file. Now rewind ...");
    ret = parser->obtain_frame(parser, p_frame);
  }

  return ret;
}

int file_parser_release_frame(void *p_parser, frame_t *p_frame)
{
  int ret;

  if (!p_parser) {
    return -1;
  }

  media_parser_t *parser = (media_parser_t *)p_parser;

  ret = parser->release_frame(parser, p_frame);

  return ret;
}

#endif //CONFIG_XOS_FWK_CADP_AGORA

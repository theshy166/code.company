#ifndef __PAGE_OVERLAY_H__
#define __PAGE_OVERLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

int page_overlay_show_video(int screen_index, const char *video_path);
int page_overlay_show_image(int screen_index, const char *image_path);
void page_overlay_raise(int screen_index);
void page_overlay_hide(int screen_index);

void page_overlay_show_demo_video(void);

#ifdef __cplusplus
}
#endif

#endif /* __PAGE_OVERLAY_H__ */

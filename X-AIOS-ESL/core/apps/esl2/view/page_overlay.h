#ifndef __PAGE_OVERLAY_H__
#define __PAGE_OVERLAY_H__

#if 0 /* -- page_overlay replaced by native MQTT-path demo (_maybe_show_demo_overlay) -- */

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

#endif /* 0 */

#endif /* __PAGE_OVERLAY_H__ */

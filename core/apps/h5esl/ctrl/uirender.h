
#ifndef __UIRENDER_H__
#define __UIRENDER_H__

void uirender_init(void);
void uirender_quit(void);

lv_obj_t * get_top_layer1(int index);
lv_obj_t * get_top_layer2(int index);

void set_top_hidden(int index);

void set_top_show(int index);
#endif

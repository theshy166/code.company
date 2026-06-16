#ifndef _QUA_BLEND_H_
#define _QUA_BLEND_H_

//
typedef struct {
    unsigned long dest_buf_phy;
    short dest_buf_w;//blend area
    short dest_buf_h;
    short dest_op_x;
    short dest_op_y;
    short dest_op_w;
    short dest_op_h;

    unsigned long src_buf_phy;
    short src_buf_w;
    short src_buf_h;
    short src_op_x;
    short src_op_y;
    short src_op_w;
    short src_op_h;
    
    //
    unsigned long bg_buf_phy;
    short bg_buf_w;
    short bg_buf_h;
    short bg_op_x;
    short bg_op_y;
    short bg_op_w;
    short bg_op_h;
    //
    short srcPreMulti;
    short bgPreMulti;
}qua_blend_dsc_t;

int qua_argb8888_blend_normal_to_argb8888_with_mask(qua_blend_dsc_t *qua_blend_dsc);
int qua_blender(qua_blend_dsc_t *qua_blend_dsc);

#endif

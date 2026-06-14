import os
import sys

def create_directories(base_dir, dirs):
    os.makedirs(base_dir, exist_ok=True)
    print(f"create dir: {base_dir}");
    for directory in dirs:
        path = os.path.join(base_dir, directory)
        os.makedirs(path, exist_ok=True)
        print(f"Created directory: {path}")

def create_file(filepath, content):
    with open(filepath, 'w') as f:
        f.write(content)
        print(f"Created file: {filepath}")

def modify_file(filepath, insert_lines, search_keyword=None):
    with open(filepath, 'r') as f:
        lines = f.readlines()

    if search_keyword:
        for i, line in enumerate(lines):
            if search_keyword in line:
                lines.insert(i, insert_lines)
                break
    else:
        lines.append(insert_lines)

    with open(filepath, 'w') as f:
        f.writelines(lines)
        print(f"Modified file: {filepath}")

def create_project(project_name):
    # Step 1: Create directories under ./core/res/XXX/
    core_res_dir = f'./core/res/{project_name}'
    create_directories(core_res_dir, ['skin', 'video', 'ttf', 'string'])

    # Step 2: Create project_XXX_conf.mk in ./product/XXX/
    product_dir = f'./product/{project_name}'
    create_directories(product_dir, [])
    mk_content = f"""export CONFIG_XOS_PROJECT_TYPE={project_name}
export CONFIG_XOS_USE_APP_{project_name.upper()}=true
export CONFIG_XOS_FWK_EFFECTS=true
export CONFIG_XOS_USE_PLAYER=true
export CONFIG_LVGL_VERSION=lvgl-v9
"""
    create_file(os.path.join(product_dir, f'project_{project_name}_conf.mk'), mk_content)
    # Step 3: Modify ./build/make/qsub/apps.mk
    apps_mk_path = './build/make/qsub/apps.mk'
    apps_mk_content = f"""ifeq ($(CONFIG_XOS_USE_APP_{project_name.upper()}),true)
APPS_CSRCS += $(shell find -L $(XOS_DIR)/$(XOS_APPS_DIR_NAME)/{project_name} -name "*.c")
endif

"""
    modify_file(apps_mk_path, apps_mk_content, "CSRCS += $(APPS_CSRCS)")

    # Step 3.1: Modify ./build/make/qsub/apps.mk
    appsmain_path = './core/apps/src/appsmain.c'
    appsmain_content = f"""#ifdef CONFIG_XOS_USE_APP_{project_name.upper()}
    extern void {project_name}_init(void);
    {project_name}_init();
#endif

"""
    modify_file(appsmain_path, appsmain_content, "//app register")

    # Step 4: Modify ./product/common/projflag.mk
    projflag_mk_path = './product/common/projflag.mk'
    projflag_mk_content = f"""
ifeq ($(CONFIG_XOS_USE_APP_{project_name.upper()}),true)
CFLAGS += -DCONFIG_XOS_USE_APP_{project_name.upper()}=1
endif
"""
    modify_file(projflag_mk_path, projflag_mk_content)

    # Step 5: Create directories and files under ./core/apps/XXX/
    core_apps_dir = f'./core/apps/{project_name}'
    create_directories(core_apps_dir, ['view', 'ctrl', 'config', 'model'])

    model_h_content = f"""#ifndef _{project_name.upper()}_HOME_OBJS_MODEL_H
#define _{project_name.upper()}_HOME_OBJS_MODEL_H

#include "lvgl/lvgl.h"

typedef struct {{
    lv_obj_t *s_container;
}} {project_name}_home_objs;

#endif //_XXX_HOME_OBJS_MODEL_H
"""
    create_file(os.path.join(core_apps_dir, 'model', f'{project_name}_home_objs_model.h'), model_h_content)

    view_h_content = f"""#ifndef _{project_name.upper()}_HOME_PAGE_H_
#define _{project_name.upper()}_HOME_PAGE_H_

//
void xos_{project_name}_wakeup(void);
void xos_{project_name}_sleep(void);
void los_{project_name}_entry(void);
//
void {project_name}_init(void);

#endif //_{project_name.upper()}_HOME_PAGE_H_
"""
    create_file(os.path.join(core_apps_dir, 'view', f'{project_name}_home_page.h'), view_h_content)

    view_c_content = f"""#include "{project_name}_home_page.h"
#include "lvgl/lvgl.h"
#include "appcommon.h"
#include "appmanager.h"
#include "qua_font.h"
#include "qm_lv_obj.h"
#include "../model/{project_name}_home_objs_model.h"
//declare
static void button_event_cb(lv_event_t * e);
static lv_obj_t * create_ui(lv_obj_t *p);

//variable:
static {project_name}_home_objs s_home_objs = {{0}};

//implements:
void xos_{project_name}_wakeup(void)
{{
}}
void xos_{project_name}_sleep(void)
{{
}}
static app_data_ptr xos_{project_name}_oncreate(void *p)
{{
    (void)p;
    los_{project_name}_entry();
    return NULL;
}}

void {project_name}_init(void) {{
    qua_app_ops *ops = malloc(sizeof(qua_app_ops));
    if (ops) {{
        memset(ops, 0x0, sizeof(qua_app_ops));
        ops->on_create = xos_{project_name}_oncreate;
        ops->on_sleep = xos_{project_name}_sleep;
        ops->on_wakeup = xos_{project_name}_wakeup;
        app_manager_register("{project_name}", ops);
    }}
}}
//implements
void los_{project_name}_entry(void)
{{
    lv_obj_t * p = lv_scr_act();
    qua_los_ft_font_init();

    s_home_objs.s_container = create_ui(p);
}}

static lv_obj_t * create_ui(lv_obj_t *p)
{{
    lv_obj_t * bg_wnd = qm_lv_obj_create(p);
    lv_obj_set_size(bg_wnd, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_obj_set_style_bg_color(bg_wnd, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(bg_wnd, LV_ALIGN_CENTER, 0, 0);
    //create button
    lv_obj_t * button = lv_button_create(bg_wnd);
    lv_obj_align(button, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_size(button, 100, 50);

    lv_obj_add_event_cb(button, button_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(button);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    //create textview
    lv_obj_t * skill_text = lv_label_create(bg_wnd);
    lv_obj_set_size(skill_text, 200, LV_USE_VER_SIZE);
    lv_label_set_text(skill_text, "Hello, world");
    lv_obj_set_style_text_align(skill_text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(skill_text, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(skill_text, LV_ALIGN_RIGHT_MID, 0, 0);
    return bg_wnd;
}}

static void button_event_cb(lv_event_t * e)
{{
    LV_LOG_USER("Clicked\\n");
}}
"""
    create_file(os.path.join(core_apps_dir, 'view', f'{project_name}_home_page.c'), view_c_content)

if __name__ == "__main__":
    if len(sys.argv) != 3 or sys.argv[1] != "-p":
        print("Usage: ./create_app.py -p XXX")
    else:
        project_name = sys.argv[2]
        create_project(project_name)


#ifndef QUA_STATUS_BAR_H
#define QUA_STATUS_BAR_H

typedef struct {
    char *text;
    char *icon_path;
} qm_statusbar_t

void qm_statusbar_init(qm_statusbar_t *objs, int obj_count);

#endif //qua_STATUS_BAR_H

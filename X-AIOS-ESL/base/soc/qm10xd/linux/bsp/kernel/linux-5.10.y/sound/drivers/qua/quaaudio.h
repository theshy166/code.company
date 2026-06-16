#ifndef __QUA_SND_H__
#define __QUA_SND_H__
#include <linux/types.h>
#include <linux/ioctl.h>


#define QUA_AUDIO_IOCTL_MAGIC 'Q'
#define QUA_AUDIO_IOCTL_DEV_OPEN _IOW(QUA_AUDIO_IOCTL_MAGIC, 0, unsigned int)
#define QUA_AUDIO_IOCTL_DEV_CLOSE _IOW(QUA_AUDIO_IOCTL_MAGIC, 1, unsigned int)

#define QUA_GPIO_HIGH_LEVEL    (1)
#define QUA_GPIO_LOW_LEVEL    (0)

typedef int (*open_func)(void* data);
typedef int (*close_func)(void* data);

struct extcon_dev {
    /* Optional user initializing data */
    const char *name;
    const unsigned int *supported_cable;
    const u32 *mutually_exclusive;

    /* Internal data. Please do not set. */
    struct device dev;
    struct raw_notifier_head nh_all;
    struct raw_notifier_head *nh;
    struct list_head entry;
    int max_supported;
    spinlock_t lock;	/* could be called by irq handler */
    u32 state;

    /* /sys/class/extcon/.../cable.n/... */
    struct device_type extcon_dev_type;
    struct extcon_cable *cables;

    /* /sys/class/extcon/.../mutually_exclusive/... */
    struct attribute_group attr_g_muex;
    struct attribute **attrs_muex;
    struct device_attribute *d_attrs_muex;
};

struct qua_audio_ops {
    open_func open_dev;
    close_func close_dev;
    void* data;
};

extern void qua_audio_ops_set(struct qua_audio_ops *ops);
#endif

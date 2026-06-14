/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *	uvc_gadget.h  --  USB Video Class Gadget driver
 *
 *	Copyright (C) 2009-2010
 *	    Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 */

#ifndef _UVC_GADGET_H_
#define _UVC_GADGET_H_

#include <linux/list.h>
#include <linux/mutex.h>
#define UVC_STILL_IMAGE_MAGIC_FLAG (V4L2_PIX_FMT_STILL_MJPEG + 0x11223344)
/* #define UVC_FASTBOOT */

#define UVC_EVENT_FIRST			(V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_CONNECT		(V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_DISCONNECT		(V4L2_EVENT_PRIVATE_START + 1)
#define UVC_EVENT_STREAMON		(V4L2_EVENT_PRIVATE_START + 2)
#define UVC_EVENT_STREAMOFF		(V4L2_EVENT_PRIVATE_START + 3)
#define UVC_EVENT_SETUP			(V4L2_EVENT_PRIVATE_START + 4)
#define UVC_EVENT_DATA			(V4L2_EVENT_PRIVATE_START + 5)
#define UVC_EVENT_LAST			(V4L2_EVENT_PRIVATE_START + 5)

struct uvc_frame_info {
	unsigned int width;
	unsigned int height;
	unsigned int intervals[8];
};

struct uvc_format_info {
	unsigned int fcc;
	const struct uvc_frame_info *frames;
	unsigned int frame_num;
};

struct uvc_fmt_array_info {
	unsigned int fmt_num;
	struct uvc_format_info *pFmts;
	unsigned int ss_fmt_num;
	struct uvc_format_info *ss_pFmts;
};

#define UVCIOC_SEND_RESPONSE		_IOW('U', 1, struct uvc_request_data)
// #define VIDIOC_CONNECT_NOW			_IOW('U', 3, struct uvc_device)
#define VIDIOC_RESET_DESC		 0x445566
#define VIDIOC_CONNECT_NOW		 0x238956
#define VIDIOC_UVC_DEMO_READY	 0x445567
#define VIDIOC_DESC_GET	         0x445568

#define UVC_INTF_CONTROL		0
#define UVC_INTF_STREAMING		1

#include <linux/spinlock.h>
#include <linux/usb/composite.h>
#include <linux/videodev2.h>
#include <linux/wait.h>

#include <media/v4l2-device.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-fh.h>

#include "uvc_queue.h"

struct usb_ep;
struct usb_request;
struct uvc_descriptor_header;
struct uvc_device;

/* ------------------------------------------------------------------------
 * Debugging, printing and logging
 */

#define UVC_TRACE_PROBE				(1 << 0)
#define UVC_TRACE_DESCR				(1 << 1)
#define UVC_TRACE_CONTROL			(1 << 2)
#define UVC_TRACE_FORMAT			(1 << 3)
#define UVC_TRACE_CAPTURE			(1 << 4)
#define UVC_TRACE_CALLS				(1 << 5)
#define UVC_TRACE_IOCTL				(1 << 6)
#define UVC_TRACE_FRAME				(1 << 7)
#define UVC_TRACE_SUSPEND			(1 << 8)
#define UVC_TRACE_STATUS			(1 << 9)

#define UVC_WARN_MINMAX				0
#define UVC_WARN_PROBE_DEF			1

extern unsigned int uvc_gadget_trace_param;

#define uvc_trace(flag, msg...) \
	do { \
		if (uvc_gadget_trace_param & flag) \
			printk(KERN_DEBUG "uvcvideo: " msg); \
	} while (0)

#define uvcg_dbg(f, fmt, args...) \
	dev_dbg(&(f)->config->cdev->gadget->dev, "%s: " fmt, (f)->name, ##args)
#define uvcg_info(f, fmt, args...) \
	dev_info(&(f)->config->cdev->gadget->dev, "%s: " fmt, (f)->name, ##args)
#define uvcg_warn(f, fmt, args...) \
	dev_warn(&(f)->config->cdev->gadget->dev, "%s: " fmt, (f)->name, ##args)
#define uvcg_err(f, fmt, args...) \
	dev_err(&(f)->config->cdev->gadget->dev, "%s: " fmt, (f)->name, ##args)

/* ------------------------------------------------------------------------
 * Driver specific constants
 */

#define UVC_NUM_REQUESTS			16
#define UVC_MAX_REQUEST_SIZE			64
#define UVC_MAX_EVENTS				4

/* ------------------------------------------------------------------------
 * Structures
 */

struct uvc_video {
	struct uvc_device *uvc;
	struct usb_ep *ep;

	struct work_struct pump;

	/* Frame parameters */
	u8 bpp;
	u32 fcc;
	unsigned int width;
	unsigned int height;
	unsigned int imagesize;
	u64 timestamp;
	struct mutex mutex;	/* protects frame parameters */

	/* Requests */
	unsigned int req_size;
	struct usb_request *req[UVC_NUM_REQUESTS];
	__u8 *req_buffer[UVC_NUM_REQUESTS];
	struct list_head req_free;
	spinlock_t req_lock;

	void (*encode) (struct usb_request *req, struct uvc_video *video,
			struct uvc_buffer *buf);

	/* Context data used by the completion handler */
	__u32 payload_size;
	__u32 max_payload_size;
	__u32 still_image_flag;

	struct uvc_video_queue queue;
	unsigned int fid;
	unsigned int stream_id;
	char *ram_buf_addr;
};

enum uvc_state {
	UVC_STATE_DISCONNECTED,
	UVC_STATE_CONNECTED,
	UVC_STATE_STREAMING,
};

struct uvc_device {
	struct video_device vdev;
	struct v4l2_device v4l2_dev;
	enum uvc_state state;
	struct usb_function func;
	struct uvc_video video;
	bool func_connected;
	wait_queue_head_t func_connected_queue;

	/* Descriptors */
	struct {
		const struct uvc_descriptor_header * const *fs_control;
		const struct uvc_descriptor_header * const *ss_control;
		const struct uvc_descriptor_header * const *fs_streaming;
		const struct uvc_descriptor_header * const *hs_streaming;
		const struct uvc_descriptor_header * const *ss_streaming;
	} desc;

	unsigned int control_intf;
	struct usb_ep *control_ep;
	struct usb_request *control_req;
	void *control_buf;

	unsigned int streaming_intf;
	bool demo_ready;
	unsigned int uvc_id;
	unsigned int bulk_stream_off;

	/* Events */
	unsigned int event_length;
	unsigned int event_setup_out : 1;
};

static inline struct uvc_device *to_uvc(struct usb_function *f)
{
	return container_of(f, struct uvc_device, func);
}
static inline struct uvc_device *video_to_uvc(struct uvc_video *v)
{
	return container_of(v, struct uvc_device, video);
}

struct uvc_file_handle {
	struct v4l2_fh vfh;
	struct uvc_video *device;
	bool is_uvc_app_handle;
};

#define to_uvc_file_handle(handle) \
	container_of(handle, struct uvc_file_handle, vfh)

/* ------------------------------------------------------------------------
 * Functions
 */

extern void uvc_function_setup_continue(struct uvc_device *uvc);
extern void uvc_endpoint_stream(struct uvc_device *dev);

extern void uvc_function_connect(struct uvc_device *uvc);
extern void uvc_function_disconnect(struct uvc_device *uvc);
extern void uvc_setup_to_demo(struct uvc_device *uvc);

extern void set_yuv_bpp(int bpp);
extern struct usb_descriptor_header **
uvc_copy_descriptors(struct uvc_device *uvc, enum usb_device_speed speed);
extern struct usb_descriptor_header **
uvc_copy_descriptors2(struct uvc_device *uvc, enum usb_device_speed speed);
#ifdef UVC_FASTBOOT
extern void uvc_events_process_class(struct uvc_device *uvc,
	struct usb_ctrlrequest *ctrl);
#endif
#endif /* _UVC_GADGET_H_ */

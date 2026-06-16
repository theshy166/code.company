// SPDX-License-Identifier: GPL-2.0+
/*
 *	webcam.c -- USB webcam gadget driver
 *
 *	Copyright (C) 2009-2010
 *	    Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/usb/video.h>

#include "u_uvc.h"

USB_GADGET_COMPOSITE_OPTIONS();

/*-------------------------------------------------------------------------*/

/* module parameters specific to the Video streaming endpoint */
static unsigned int streaming_interval = 1;
module_param(streaming_interval, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(streaming_interval, "1 - 16");

#ifdef CONFIG_UVC_EP_TYPE_BULK
static unsigned int streaming_maxpacket = 1024;
#else
#ifdef CONFIG_UVC_HIGH_BANDWIDTH
static unsigned int streaming_maxpacket = 1024 * 3;
#else
static unsigned int streaming_maxpacket = 1024;
#endif
#endif
module_param(streaming_maxpacket, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(streaming_maxpacket, "1 - 1023 (FS), 1 - 3072 (hs/ss)");

static unsigned int streaming_maxburst = 6;
module_param(streaming_maxburst, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(streaming_maxburst, "0 - 15 (ss only)");

/* --------------------------------------------------------------------------
 * Device descriptor
 */

#define WEBCAM_VENDOR_ID		0x1d6b	/* Linux Foundation */
#define WEBCAM_PRODUCT_ID		0x0102	/* Webcam A/V gadget */
#define WEBCAM_DEVICE_BCD		0x0010	/* 0.10 */

static char webcam_vendor_label[] = "Linux Foundation";
static char webcam_product_label[] = "Webcam gadget";
static char webcam_config_label[] = "Video";

/* string IDs are assigned dynamically */

#define STRING_DESCRIPTION_IDX		USB_GADGET_FIRST_AVAIL_IDX

#ifdef CONFIG_UVC_WINDOWS_HELLO_FACE
#define MSXU_EXTENSION_UNIT_CONTROLS (0x01e0)
#else
#define MSXU_EXTENSION_UNIT_CONTROLS (0x0)
#endif
static struct usb_string webcam_strings[] = {
	[USB_GADGET_MANUFACTURER_IDX].s = webcam_vendor_label,
	[USB_GADGET_PRODUCT_IDX].s = webcam_product_label,
	[USB_GADGET_SERIAL_IDX].s = "12345678",
	[STRING_DESCRIPTION_IDX].s = webcam_config_label,
	{  }
};

static struct usb_gadget_strings webcam_stringtab = {
	.language = 0x0409,	/* en-us */
	.strings = webcam_strings,
};

static struct usb_gadget_strings *webcam_device_strings[] = {
	&webcam_stringtab,
	NULL,
};

static struct usb_function_instance *fi_uvc;
static struct usb_function *f_uvc;
#ifdef CONFIG_UVC_DOUBLE_STREAM
static unsigned int streaming_interval2 = 1;
#ifdef CONFIG_UVC_EP_TYPE_BULK
static unsigned int streaming_maxpacket2 = 1024;
#else
#ifdef CONFIG_UVC_HIGH_BANDWIDTH
static unsigned int streaming_maxpacket2 = 1024 * 3;
#else
static unsigned int streaming_maxpacket2 = 1024;
#endif
#endif
static unsigned int streaming_maxburst2 = 6;
static struct usb_function_instance *fi_uvc2;
static struct usb_function *f_uvc2;
#endif

static struct usb_device_descriptor webcam_device_descriptor = {
	.bLength		= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	/* .bcdUSB = DYNAMIC */
	.bDeviceClass		= USB_CLASS_MISC,
	.bDeviceSubClass	= 0x02,
	.bDeviceProtocol	= 0x01,
	.bMaxPacketSize0	= 0, /* dynamic */
	.idVendor		= cpu_to_le16(WEBCAM_VENDOR_ID),
	.idProduct		= cpu_to_le16(WEBCAM_PRODUCT_ID),
	.bcdDevice		= cpu_to_le16(WEBCAM_DEVICE_BCD),
	.iManufacturer		= 0, /* dynamic */
	.iProduct		= 0, /* dynamic */
	.iSerialNumber		= 0, /* dynamic */
	.bNumConfigurations	= 0, /* dynamic */
};

DECLARE_UVC_HEADER_DESCRIPTOR(2);

static const struct UVC_HEADER_DESCRIPTOR(2) uvc_control_header = {
	.bLength		= UVC_DT_HEADER_SIZE(1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_HEADER,
	.bcdUVC			= cpu_to_le16(UVC_CLASS_CODE),
	.wTotalLength		= 0, /* dynamic */
	.dwClockFrequency	= cpu_to_le32(48000000),
	.bInCollection		= 0, /* dynamic */
	.baInterfaceNr[0]	= 0, /* dynamic */
};

static const struct uvc_camera_terminal_descriptor uvc_camera_terminal = {
	.bLength		= UVC_DT_CAMERA_TERMINAL_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_INPUT_TERMINAL,
	.bTerminalID		= UVC_CAMERAL_TERMINAL_ID,
	.wTerminalType		= cpu_to_le16(0x0201),
	.bAssocTerminal		= 0,
	.iTerminal		= 0,
	.wObjectiveFocalLengthMin	= cpu_to_le16(0),
	.wObjectiveFocalLengthMax	= cpu_to_le16(0),
	.wOcularFocalLength		= cpu_to_le16(0),
	.bControlSize		= 3,
	.bmControls[0]		= 0x0e,
	.bmControls[1]		= 0x00,
	.bmControls[2]		= 0x30,
};

static const struct uvc_processing_unit_descriptor uvc_processing = {
	.bLength		= UVC_PROCESS_UNIT_DESC_LEN,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_PROCESSING_UNIT,
	.bUnitID		= UVC_PROCESSING_UNIT_ID,
	.bSourceID		= UVC_CAMERAL_TERMINAL_ID,
	.wMaxMultiplier		= cpu_to_le16(16*1024),
	.bControlSize		= 3,
	.bmControls[0]		= 0x7f,
	.bmControls[1]		= 0x15,
	.bmControls[2]		= 0x00,
	.iProcessing		= 0,
	.bmVideoStandards	= 0,
};
DECLARE_UVC_EXTENSION_UNIT_DESCRIPTOR(1, 1);
static const struct UVC_EXTENSION_UNIT_DESCRIPTOR(1, 1) h264_extension_unit = {
	.bLength		= UVC_DT_EXTENSION_UNIT_SIZE(1, 1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_EXTENSION_UNIT,
	.bUnitID		= UVC_H264_EXTENSION_UNIT_ID,
	.guidExtensionCode = { 0x41, 0x76, 0x9e, 0xa2, 0x04, 0xde, 0xe3,
	0x47, 0x8b, 0x2b, 0xf4, 0x34, 0x1a, 0xff, 0x88, 0x88},
	.bNumControls = 8,
	.bNrInPins = 1,
	.baSourceID[0]		= UVC_PROCESSING_UNIT_ID,
	.bControlSize		= 1,
	.bmControls[0]		= 0xff,
	.iExtension		= 0,
};

static const struct uvc_output_terminal_descriptor uvc_output_terminal = {
	.bLength		= UVC_DT_OUTPUT_TERMINAL_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_OUTPUT_TERMINAL,
	.bTerminalID		= UVC_OUTPUT_TERMINAL_ID,
	.wTerminalType		= cpu_to_le16(0x0101),
	.bAssocTerminal		= 0,
	.bSourceID		= UVC_H264_EXTENSION_UNIT_ID,
	.iTerminal		= 0,
};
#ifdef CONFIG_UVC_DOUBLE_STREAM
static const struct UVC_HEADER_DESCRIPTOR(2) uvc_control_header2 = {
	.bLength		= UVC_DT_HEADER_SIZE(1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_HEADER,
	.bcdUVC			= cpu_to_le16(UVC_CLASS_CODE),
	.wTotalLength		= 0, /* dynamic */
	.dwClockFrequency	= cpu_to_le32(48000000),
	.bInCollection		= 0, /* dynamic */
	.baInterfaceNr[0]	= 0, /* dynamic */
};
static const struct uvc_camera_terminal_descriptor uvc_camera_terminal2 = {
	.bLength		= UVC_DT_CAMERA_TERMINAL_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_INPUT_TERMINAL,
	.bTerminalID		= UVC_CAMERAL_TERMINAL_ID,
	.wTerminalType		= cpu_to_le16(0x0201),
	.bAssocTerminal		= 0,
	.iTerminal		= 0,
	.wObjectiveFocalLengthMin	= cpu_to_le16(0),
	.wObjectiveFocalLengthMax	= cpu_to_le16(0),
	.wOcularFocalLength		= cpu_to_le16(0),
	.bControlSize		= 3,
	.bmControls[0]		= 0x00,
	.bmControls[1]		= 0x00,
	.bmControls[2]		= 0x30,
};
static const struct uvc_processing_unit_descriptor uvc_processing2 = {
	.bLength		= UVC_PROCESS_UNIT_DESC_LEN,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_PROCESSING_UNIT,
	.bUnitID		= UVC_PROCESSING_UNIT_ID,
	.bSourceID		= UVC_CAMERAL_TERMINAL_ID,
	.wMaxMultiplier		= cpu_to_le16(16*1024),
	.bControlSize		= 3,
	.bmControls[0]		= 0x00,
	.bmControls[1]		= 0x00,
	.bmControls[2]		= 0x00,
	.iProcessing		= 0,
};
DECLARE_UVC_EXTENSION_UNIT_DESCRIPTOR(1, 4);
static const struct UVC_EXTENSION_UNIT_DESCRIPTOR(1, 4)
	msxu_face_extension_unit2 = {
	.bLength        = UVC_DT_EXTENSION_UNIT_SIZE(1, 4),
	.bDescriptorType    = USB_DT_CS_INTERFACE,
	.bDescriptorSubType    = UVC_VC_EXTENSION_UNIT,
	.bUnitID        = UVC_MSXU_FACE_EXTENSION_UNIT_ID,
	.guidExtensionCode = { 0xdc, 0x95, 0x3f, 0x0f, 0x32,
	0x26, 0x4e, 0x4c, 0x92, 0xc9, 0xa0, 0x47, 0x82,
	0xf4, 0x3b, 0xc8},
	.bNumControls = 0x04,
	.bNrInPins = 1,
	.baSourceID[0]        = UVC_PROCESSING_UNIT_ID,
	.bControlSize        = 4,
	.bmControls[0]        = MSXU_EXTENSION_UNIT_CONTROLS & 0xff,
	.bmControls[1]        = (MSXU_EXTENSION_UNIT_CONTROLS >> 8) & 0xff,
	.bmControls[2]        = (MSXU_EXTENSION_UNIT_CONTROLS >> 16) & 0xff,
	.bmControls[3]        = (MSXU_EXTENSION_UNIT_CONTROLS >> 24) & 0xff,
	.iExtension        = 0,
};


static const struct uvc_output_terminal_descriptor uvc_output_terminal2 = {
	.bLength		= UVC_DT_OUTPUT_TERMINAL_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VC_OUTPUT_TERMINAL,
	.bTerminalID		= UVC_OUTPUT_TERMINAL_ID,
	.wTerminalType		= cpu_to_le16(0x0101),
	.bAssocTerminal		= 0,
	.bSourceID		= UVC_PROCESSING_UNIT_ID,
	.iTerminal		= 0,
};
#endif

DECLARE_UVC_INPUT_HEADER_DESCRIPTOR(1, 1);

static const struct UVC_INPUT_HEADER_DESCRIPTOR(1, 1) uvc_input_header = {
	.bLength		= UVC_DT_INPUT_HEADER_SIZE(1, 1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_INPUT_HEADER,
	.bNumFormats		= 1,
	.wTotalLength		= 0, /* dynamic */
	.bEndpointAddress	= 0, /* dynamic */
	.bmInfo			= 0,
	.bTerminalLink		= UVC_OUTPUT_TERMINAL_ID,
	.bStillCaptureMethod	= 0,
	.bTriggerSupport	= 0,
	.bTriggerUsage		= 0,
	.bControlSize		= 1,
	.bmaControls[0][0]	= 0,
	// .bmaControls[1][0]	= 4,
};

static const struct uvc_format_uncompressed uvc_format_yuv = {
	.bLength		= UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_UNCOMPRESSED,
	.bFormatIndex		= 1,
	.bNumFrameDescriptors	= 2,
	.guidFormat		= {
		 'Y',  'U',  'Y',  '2', 0x00, 0x00, 0x10, 0x00,
		 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
	.bBitsPerPixel		= 16,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
};

DECLARE_UVC_FRAME_UNCOMPRESSED(1);
DECLARE_UVC_FRAME_UNCOMPRESSED(3);

static const struct UVC_FRAME_UNCOMPRESSED(3) uvc_frame_yuv_360p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(640),
	.wHeight		= cpu_to_le16(360),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static const struct UVC_FRAME_UNCOMPRESSED(1) uvc_frame_yuv_720p = {
	.bLength		= UVC_DT_FRAME_UNCOMPRESSED_SIZE(1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_UNCOMPRESSED,
	.bFrameIndex		= 2,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1280),
	.wHeight		= cpu_to_le16(720),
	.dwMinBitRate		= cpu_to_le32(29491200),
	.dwMaxBitRate		= cpu_to_le32(29491200),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(1843200),
	.dwDefaultFrameInterval	= cpu_to_le32(5000000),
	.bFrameIntervalType	= 1,
	.dwFrameInterval[0]	= cpu_to_le32(5000000),
};

static const struct uvc_format_mjpeg uvc_format_mjpg = {
	.bLength		= UVC_DT_FORMAT_MJPEG_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FORMAT_MJPEG,
	.bFormatIndex		= 2,
	.bNumFrameDescriptors	= 2,
	.bmFlags		= 0,
	.bDefaultFrameIndex	= 1,
	.bAspectRatioX		= 0,
	.bAspectRatioY		= 0,
	.bmInterfaceFlags	= 0,
	.bCopyProtect		= 0,
};

DECLARE_UVC_FRAME_MJPEG(1);
DECLARE_UVC_FRAME_MJPEG(3);

static const struct UVC_FRAME_MJPEG(3) uvc_frame_mjpg_360p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(3),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 1,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(640),
	.wHeight		= cpu_to_le16(360),
	.dwMinBitRate		= cpu_to_le32(18432000),
	.dwMaxBitRate		= cpu_to_le32(55296000),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(460800),
	.dwDefaultFrameInterval	= cpu_to_le32(666666),
	.bFrameIntervalType	= 3,
	.dwFrameInterval[0]	= cpu_to_le32(666666),
	.dwFrameInterval[1]	= cpu_to_le32(1000000),
	.dwFrameInterval[2]	= cpu_to_le32(5000000),
};

static const struct UVC_FRAME_MJPEG(1) uvc_frame_mjpg_720p = {
	.bLength		= UVC_DT_FRAME_MJPEG_SIZE(1),
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_FRAME_MJPEG,
	.bFrameIndex		= 2,
	.bmCapabilities		= 0,
	.wWidth			= cpu_to_le16(1280),
	.wHeight		= cpu_to_le16(720),
	.dwMinBitRate		= cpu_to_le32(29491200),
	.dwMaxBitRate		= cpu_to_le32(29491200),
	.dwMaxVideoFrameBufferSize	= cpu_to_le32(1843200),
	.dwDefaultFrameInterval	= cpu_to_le32(5000000),
	.bFrameIntervalType	= 1,
	.dwFrameInterval[0]	= cpu_to_le32(5000000),
};

static const struct uvc_color_matching_descriptor uvc_color_matching = {
	.bLength		= UVC_DT_COLOR_MATCHING_SIZE,
	.bDescriptorType	= USB_DT_CS_INTERFACE,
	.bDescriptorSubType	= UVC_VS_COLORFORMAT,
	.bColorPrimaries	= 1,
	.bTransferCharacteristics	= 1,
	.bMatrixCoefficients	= 4,
};

static const struct uvc_descriptor_header * const uvc_fs_control_cls[] = {
	(const struct uvc_descriptor_header *) &uvc_control_header,
	(const struct uvc_descriptor_header *) &uvc_camera_terminal,
	(const struct uvc_descriptor_header *) &uvc_processing,
	(const struct uvc_descriptor_header *) &h264_extension_unit,
	(const struct uvc_descriptor_header *) &uvc_output_terminal,
	NULL,
};

static const struct uvc_descriptor_header * const uvc_ss_control_cls[] = {
	(const struct uvc_descriptor_header *) &uvc_control_header,
	(const struct uvc_descriptor_header *) &uvc_camera_terminal,
	(const struct uvc_descriptor_header *) &uvc_processing,
	(const struct uvc_descriptor_header *) &h264_extension_unit,
	(const struct uvc_descriptor_header *) &uvc_output_terminal,
	NULL,
};
#ifdef CONFIG_UVC_DOUBLE_STREAM
static const struct uvc_descriptor_header * const uvc_fs_control_cls2[] = {
	(const struct uvc_descriptor_header *) &uvc_control_header2,
	(const struct uvc_descriptor_header *) &uvc_camera_terminal2,
	(const struct uvc_descriptor_header *) &uvc_processing2,
	(const struct uvc_descriptor_header *) &msxu_face_extension_unit2,
	(const struct uvc_descriptor_header *) &uvc_output_terminal2,
	NULL,
};
static const struct uvc_descriptor_header * const uvc_ss_control_cls2[] = {
	(const struct uvc_descriptor_header *) &uvc_control_header2,
	(const struct uvc_descriptor_header *) &uvc_camera_terminal2,
	(const struct uvc_descriptor_header *) &uvc_processing2,
	(const struct uvc_descriptor_header *) &msxu_face_extension_unit2,
	(const struct uvc_descriptor_header *) &uvc_output_terminal2,
	NULL,
};
#endif

static const struct uvc_descriptor_header * const uvc_fs_streaming_cls[] = {
	(const struct uvc_descriptor_header *) &uvc_input_header,
	(const struct uvc_descriptor_header *) &uvc_format_yuv,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_720p,
	(const struct uvc_descriptor_header *) &uvc_format_mjpg,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_720p,
	(const struct uvc_descriptor_header *) &uvc_color_matching,
	NULL,
};

static const struct uvc_descriptor_header * const uvc_hs_streaming_cls[] = {
	(const struct uvc_descriptor_header *) &uvc_input_header,
	(const struct uvc_descriptor_header *) &uvc_format_yuv,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_720p,
	(const struct uvc_descriptor_header *) &uvc_format_mjpg,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_720p,
	(const struct uvc_descriptor_header *) &uvc_color_matching,
	NULL,
};

static const struct uvc_descriptor_header * const uvc_ss_streaming_cls[] = {
	(const struct uvc_descriptor_header *) &uvc_input_header,
	(const struct uvc_descriptor_header *) &uvc_format_yuv,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_yuv_720p,
	(const struct uvc_descriptor_header *) &uvc_format_mjpg,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_360p,
	(const struct uvc_descriptor_header *) &uvc_frame_mjpg_720p,
	(const struct uvc_descriptor_header *) &uvc_color_matching,
	NULL,
};
#ifdef CONFIG_UVC_WINDOWS_HELLO_FACE
static unsigned char vendor_string[] = {
/* Microsoft OS 2.0 Descriptor Set Header */
0x0A, 0x00, /* wLength of MSOS20_SET_HEADER_DESCRIPTOR */
0x00, 0x00, /* wDescriptorType == MSOS20_SET_HEADER_DESCRIPTOR */
0x00, 0x00, 0x00, 0x0A, /* dwWindowsVersion �C 0x10000000 for Windows 10 */
0x6A, 0x03, /* wTotalLength - Total length */

/* Microsoft OS 2.0 Configuration Subset Header */
0x08, 0x00, /* wLength of MSOS20_SUBSET_HEADER_CONFIGURATION */
0x01, 0x00, /* wDescriptorType == MSOS20_SUBSET_HEADER_CONFIGURATION */
0x00, /* bConfigurationValue set to the first configuration */
0x00, /* bReserved set to 0. */
0x60, 0x03, /* wTotalLength - Total length */

/****************Color Camera Function******************/
/* Microsoft OS 2.0 Function Subset Header */
0x08, 0x00, /* wLength of MSOS20_SUBSET_HEADER_FUNCTION */
0x02, 0x00, /* wDescriptorType == MSOS20_SUBSET_HEADER_FUNCTION */
0x00, /* bFirstInterface field of the first IAD */
0x00, /* bReserved set to 0. */
0xDE, 0x01, /* wSubsetLength - Length */

/****************Register the Color Camera in a sensor group******************/
/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x80, 0x00, /* wLength 0x80 (128) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x01, 0x00, /* wPropertyDataType - REG_SZ */
0x28, 0x00, /* wPropertyNameLength �C 0x28 (40) bytes */
/* Property Name - "UVC-FSSensorGroupID" */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'F', 0x00, 'S', 0x00, 'S', 0x00, 'e', 0x00,
'n', 0x00, 's', 0x00, 'o', 0x00, 'r', 0x00,
'G', 0x00, 'r', 0x00, 'o', 0x00, 'u', 0x00,
'p', 0x00, 'I', 0x00, 'D', 0x00, 0x00, 0x00,
0x4E, 0x00, /* wPropertyDataLength �C 0x4E (78) bytes */

/* FSSensorGroupID GUID in string format: */
'{', 0x00, '1', 0x00, '3', 0x00, '1', 0x00,
'F', 0x00, 'F', 0x00, '5', 0x00, '7', 0x00,
'B', 0x00, '-', 0x00, '1', 0x00, 'F', 0x00,
'5', 0x00, '3', 0x00, '-', 0x00, '4', 0x00,
'9', 0x00, '6', 0x00, '3', 0x00, '-', 0x00,
'8', 0x00, '6', 0x00, '0', 0x00, 'E', 0x00,
'-', 0x00, '9', 0x00, 'F', 0x00, '1', 0x00,
'9', 0x00, '0', 0x00, '0', 0x00, '6', 0x00,
'3', 0x00, 'C', 0x00, '2', 0x00, 'C', 0x00,
'8', 0x00, '}', 0x00, 0x00, 0x00,

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x56, 0x00, /* wLength 0x56 (86) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x01, 0x00, /* wPropertyDataType - REG_SZ */
0x2C, 0x00, /* wPropertyNameLength �C 0x2C (44) bytes */
/* Property Name - "UVC-FSSensorGroupName" */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'F', 0x00, 'S', 0x00, 'S', 0x00, 'e', 0x00,
'n', 0x00, 's', 0x00, 'o', 0x00, 'r', 0x00,
'G', 0x00, 'r', 0x00, 'o', 0x00, 'u', 0x00,
'p', 0x00, 'N', 0x00, 'a', 0x00, 'm', 0x00,
'e', 0x00, 0x00, 0x00,
0x20, 0x00, /* wPropertyDataLength �C 0x20 (32) bytes */
/* FSSensorGroupName "FhygCameraGroup" */
'S', 0x00, 'p', 0x00, 'i', 0x00, 't', 0x00,
'C', 0x00, 'a', 0x00, 'm', 0x00, 'e', 0x00,
'r', 0x00, 'a', 0x00, 'G', 0x00, 'r', 0x00,
'o', 0x00, 'u', 0x00, 'p', 0x00, 0x00, 0x00,

/****************disable Still Image Capture for Color Camera************/
/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x54, 0x00, /* wLength 0x54 (84) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x46, 0x00, /* wPropertyNameLength �C 0x46 (70) bytes */
/* Property Name - "UVC-EnableDependentStillPinCapture" */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'E', 0x00, 'n', 0x00, 'a', 0x00, 'b', 0x00,
'l', 0x00, 'e', 0x00, 'D', 0x00, 'e', 0x00,
'p', 0x00, 'e', 0x00, 'n', 0x00, 'd', 0x00,
'e', 0x00, 'n', 0x00, 't', 0x00, 'S', 0x00,
't', 0x00, 'i', 0x00, 'l', 0x00, 'l', 0x00,
'P', 0x00, 'i', 0x00, 'n', 0x00, 'C', 0x00,
'a', 0x00, 'p', 0x00, 't', 0x00, 'u', 0x00,
'r', 0x00, 'e', 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
/* disable still pin capture using Method 2 or Method 3 */
0x00, 0x00, 0x00, 0x00,

/****************Enable Platform DMFT for ROI-capable USB Camera************/
/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x3C, 0x00, /* wLength 0x3C (60) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x2E, 0x00, /* wPropertyNameLength �C 0x2E (46) bytes */
/* Property Name - "UVC-EnablePlatformDmft" */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'E', 0x00, 'n', 0x00, 'a', 0x00, 'b', 0x00,
'l', 0x00, 'e', 0x00, 'P', 0x00, 'l', 0x00,
'a', 0x00, 't', 0x00, 'f', 0x00, 'o', 0x00,
'r', 0x00, 'm', 0x00, 'D', 0x00, 'm', 0x00,
'f', 0x00, 't', 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
0x01, 0x00, 0x00, 0x00, /* Enable Platform DMFT */

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x3E, 0x00, /* wLength in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x30, 0x00, /* wPropertyNameLength*/
/* Property Name - "EnableDshowRedirection" */
'E', 0x00, 'n', 0x00, 'a', 0x00, 'b', 0x00,
'l', 0x00, 'e', 0x00, 'D', 0x00, 's', 0x00,
'h', 0x00, 'o', 0x00, 'w', 0x00, 'R', 0x00,
'e', 0x00, 'd', 0x00, 'i', 0x00, 'r', 0x00,
'e', 0x00, 'c', 0x00, 't', 0x00, 'i', 0x00,
'o', 0x00, 'n', 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
0x01, 0x00, 0x00, 0x00, /* Enable Dshow bridge */

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x32, 0x00, /* wLength in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x24, 0x00, /* wPropertyNameLength*/
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'C', 0x00, 'P', 0x00, 'V', 0x00, '2', 0x00,
'F', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00,
'A', 0x00, 'u', 0x00, 't', 0x00, 'h', 0x00,
0x00, 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
0xFF, 0xFF, 0x00, 0x00, /* RGB camera payload format config */

/****************IR Camera Function**************/
/* Microsoft OS 2.0 Function Subset Header */
0x08, 0x00, /* wLength of MSOS20_SUBSET_HEADER_FUNCTION */
0x02, 0x00, /* wDescriptorType == MSOS20_SUBSET_HEADER_FUNCTION */
0x02, /* bFirstInterface set of the second function */
0x00,
0x7A, 0x01, /* wSubsetLength */

/*
 * Register the IR Camera to the
 * same sensor group as the Color Camera
 */
/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x80, 0x00, /* wLength 0x80 (128) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x01, 0x00, /* wPropertyDataType - REG_SZ */
0x28, 0x00, /* wPropertyNameLength �C 0x28 (40) bytes */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'F', 0x00, 'S', 0x00, 'S', 0x00, 'e', 0x00,
'n', 0x00, 's', 0x00, 'o', 0x00, 'r', 0x00,
'G', 0x00, 'r', 0x00, 'o', 0x00, 'u', 0x00,
'p', 0x00, 'I', 0x00, 'D', 0x00, 0x00, 0x00,
0x4E, 0x00, /* wPropertyDataLength */

/* FSSensorGroupID GUID in string format: */
'{', 0x00, '1', 0x00, '3', 0x00, '1', 0x00,
'F', 0x00, 'F', 0x00, '5', 0x00, '7', 0x00,
'B', 0x00, '-', 0x00, '1', 0x00, 'F', 0x00,
'5', 0x00, '3', 0x00, '-', 0x00, '4', 0x00,
'9', 0x00, '6', 0x00, '3', 0x00, '-', 0x00,
'8', 0x00, '6', 0x00, '0', 0x00, 'E', 0x00,
'-', 0x00, '9', 0x00, 'F', 0x00, '1', 0x00,
'9', 0x00, '0', 0x00, '0', 0x00, '6', 0x00,
'3', 0x00, 'C', 0x00, '2', 0x00, 'C', 0x00,
'8', 0x00, '}', 0x00, 0x00, 0x00,

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x56, 0x00, /* wLength 0x56 (86) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x01, 0x00, /* wPropertyDataType - REG_SZ */
0x2C, 0x00, /* wPropertyNameLength �C 0x2C (44) bytes */
/* Property Name - "UVC-FSSensorGroupName" */
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'F', 0x00, 'S', 0x00, 'S', 0x00, 'e', 0x00,
'n', 0x00, 's', 0x00, 'o', 0x00, 'r', 0x00,
'G', 0x00, 'r', 0x00, 'o', 0x00, 'u', 0x00,
'p', 0x00, 'N', 0x00, 'a', 0x00, 'm', 0x00,
'e', 0x00, 0x00, 0x00,
0x20, 0x00, /* wPropertyDataLength �C 32 bytes */
/* FSSensorGroupName "FhygCameraGroup" */
'S', 0x00, 'p', 0x00, 'i', 0x00, 't', 0x00,
'C', 0x00, 'a', 0x00, 'm', 0x00, 'e', 0x00,
'r', 0x00, 'a', 0x00, 'G', 0x00, 'r', 0x00,
'o', 0x00, 'u', 0x00, 'p', 0x00, 0x00, 0x00,

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x30, 0x00, /* wLength 0x30 (48) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x22, 0x00, /* wPropertyNameLength �C 0x22 (34) bytes */
/* Property Name - "SensorCameraMode" */
'S', 0x00, 'e', 0x00, 'n', 0x00, 's', 0x00,
'o', 0x00, 'r', 0x00, 'C', 0x00, 'a', 0x00,
'm', 0x00, 'e', 0x00, 'r', 0x00, 'a', 0x00,
'M', 0x00, 'o', 0x00, 'd', 0x00, 'e', 0x00,
0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
/* This exposes the camera to OS as an IR only camera */
0x01, 0x00, 0x00, 0x00,
/* i.e. KSCATEGORY_SENSOR_CAMERA */

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x3A, 0x00, /* wLength 0x3A (58) in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x2C, 0x00, /* wPropertyNameLength �C 0x2C (44) bytes */
/* Property Name - "SkipCameraEnumeration" */
'S', 0x00, 'k', 0x00, 'i', 0x00, 'p', 0x00,
'C', 0x00, 'a', 0x00, 'm', 0x00, 'e', 0x00,
'r', 0x00, 'a', 0x00, 'E', 0x00, 'n', 0x00,
'u', 0x00, 'm', 0x00, 'e', 0x00, 'r', 0x00,
'a', 0x00, 't', 0x00, 'i', 0x00, 'o', 0x00,
'n', 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
/* This exposes the camera to applications looking for IR only cameras */
0x01, 0x00, 0x00, 0x00,

/* Microsoft OS 2.0 Registry Value Feature Descriptor */
0x32, 0x00, /* wLength in bytes of this descriptor */
0x04, 0x00, /* wDescriptorType �C MSOS20_FEATURE_REG_PROPERTY */
0x04, 0x00, /* wPropertyDataType - REG_DWORD */
0x24, 0x00,
'U', 0x00, 'V', 0x00, 'C', 0x00, '-', 0x00,
'C', 0x00, 'P', 0x00, 'V', 0x00, '2', 0x00,
'F', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00,
'A', 0x00, 'u', 0x00, 't', 0x00, 'h', 0x00,
0x00, 0x00, 0x00, 0x00,
0x04, 0x00, /* wPropertyDataLength �C 4 bytes */
0x00, 0x00, 0xFF, 0xFF
};
#endif
/* --------------------------------------------------------------------------
 * USB configuration
 */

static int
webcam_config_bind(struct usb_configuration *c)
{
	int status = 0;

	f_uvc = usb_get_function(fi_uvc);
	if (IS_ERR(f_uvc))
		return PTR_ERR(f_uvc);

	status = usb_add_function(c, f_uvc);
	if (status < 0)
		usb_put_function(f_uvc);
#ifdef CONFIG_UVC_DOUBLE_STREAM
	f_uvc2 = usb_get_function(fi_uvc2);
	if (IS_ERR(f_uvc2))
		return PTR_ERR(f_uvc2);
	status = usb_add_function(c, f_uvc2);
	if (status < 0)
		usb_put_function(f_uvc2);
#endif

	return status;
}

static struct usb_configuration webcam_config_driver = {
	.label			= webcam_config_label,
	.bConfigurationValue	= 1,
	.iConfiguration		= 0, /* dynamic */
	.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
	.MaxPower		= CONFIG_USB_GADGET_VBUS_DRAW,
};

static int
webcam_unbind(struct usb_composite_dev *cdev)
{
	if (!IS_ERR_OR_NULL(f_uvc)) {
		usb_put_function(f_uvc);
		f_uvc = NULL;
	}
	if (!IS_ERR_OR_NULL(fi_uvc)) {
		usb_put_function_instance(fi_uvc);
		fi_uvc = NULL;
	}
#ifdef CONFIG_UVC_DOUBLE_STREAM
	if (!IS_ERR_OR_NULL(f_uvc2)) {
		usb_put_function(f_uvc2);
		f_uvc2 = NULL;
	}
	if (!IS_ERR_OR_NULL(fi_uvc2)) {
		usb_put_function_instance(fi_uvc2);
		fi_uvc2 = NULL;
	}
#endif
	return 0;
}

static int
webcam_bind(struct usb_composite_dev *cdev)
{
	struct f_uvc_opts *uvc_opts;
	int ret;

	fi_uvc = usb_get_function_instance("uvc");
	if (IS_ERR(fi_uvc))
		return PTR_ERR(fi_uvc);

	uvc_opts = container_of(fi_uvc, struct f_uvc_opts, func_inst);

	uvc_opts->streaming_interval = streaming_interval;
	uvc_opts->streaming_maxpacket = streaming_maxpacket;
	uvc_opts->streaming_maxburst = streaming_maxburst;

	uvc_opts->fs_control = uvc_fs_control_cls;
	uvc_opts->ss_control = uvc_ss_control_cls;
	uvc_opts->fs_streaming = uvc_fs_streaming_cls;
	uvc_opts->hs_streaming = uvc_hs_streaming_cls;
	uvc_opts->ss_streaming = uvc_ss_streaming_cls;

#ifdef CONFIG_UVC_DOUBLE_STREAM
	fi_uvc2 = usb_get_function_instance("uvc");
	if (IS_ERR(fi_uvc2))
		return PTR_ERR(fi_uvc2);
	uvc_opts = container_of(fi_uvc2, struct f_uvc_opts, func_inst);
	uvc_opts->streaming_interval = streaming_interval2;
	uvc_opts->streaming_maxpacket = streaming_maxpacket2;
	uvc_opts->streaming_maxburst = streaming_maxburst2;
	uvc_opts->fs_control = uvc_fs_control_cls2;
	uvc_opts->ss_control = uvc_ss_control_cls2;
	uvc_opts->fs_streaming = uvc_fs_streaming_cls;
	uvc_opts->hs_streaming = uvc_hs_streaming_cls;
	uvc_opts->ss_streaming = uvc_ss_streaming_cls;
#endif
	/* Allocate string descriptor numbers ... note that string contents
	 * can be overridden by the composite_dev glue.
	 */
	ret = usb_string_ids_tab(cdev, webcam_strings);
	if (ret < 0)
		goto error;
	webcam_device_descriptor.iManufacturer =
		webcam_strings[USB_GADGET_MANUFACTURER_IDX].id;
	webcam_device_descriptor.iProduct =
		webcam_strings[USB_GADGET_PRODUCT_IDX].id;
		webcam_device_descriptor.iSerialNumber =
		webcam_strings[USB_GADGET_SERIAL_IDX].id;

	/* Register our configuration. */
	if ((ret = usb_add_config(cdev, &webcam_config_driver,
					webcam_config_bind)) < 0)
		goto error;

	usb_composite_overwrite_options(cdev, &coverwrite);
	INFO(cdev, "Webcam Video Gadget\n");
#ifdef CONFIG_UVC_WINDOWS_HELLO_FACE
/* Microsoft OS Descriptor support,by mengqx352 */
	unsigned char Mtfs100[OS_STRING_QW_SIGN_LEN] = {0x4d, 0x00,
	0x53, 0x00, 0x46, 0x00, 0x54, 0x00, 0x31, 0x00, 0x30,
	0x00, 0x30, 0x00};

	cdev->use_os_string = true;
	cdev->b_vendor_code = 0x01;
	memcpy(cdev->qw_sign, Mtfs100, OS_STRING_QW_SIGN_LEN);
	memcpy(cdev->msos_feature_desc, vendor_string, sizeof(vendor_string));
	cdev->msos_feature_length = sizeof(vendor_string);
#endif
	return 0;

error:
	usb_put_function_instance(fi_uvc);
	fi_uvc = NULL;
#ifdef CONFIG_UVC_DOUBLE_STREAM
	usb_put_function_instance(fi_uvc2);
	fi_uvc2 = NULL;
#endif
	return ret;
}

/* --------------------------------------------------------------------------
 * Driver
 */

static struct usb_composite_driver webcam_driver = {
	.name		= "g_webcam",
	.dev		= &webcam_device_descriptor,
	.strings	= webcam_device_strings,
	.max_speed	= USB_SPEED_SUPER,
	.bind		= webcam_bind,
	.unbind		= webcam_unbind,
};

module_usb_composite_driver(webcam_driver);

MODULE_AUTHOR("Laurent Pinchart");
MODULE_DESCRIPTION("Webcam Video Gadget");
MODULE_LICENSE("GPL");


/*
 * usb_drv.h
 *
 *  Created on: 2018
 *      Author: jason.qin
 */

#ifndef MODULES_USB_DRV_H_
#define MODULES_USB_DRV_H_

#define out	(1 == 0)
#define in	(1 == 1)

#define USB_DIR_OUT			0		
#define USB_DIR_IN			0x80

#define EP0_MPS_LIMIT   64 
#define PHY_IF GUSBCFG_PHYIF8

enum dwc2_ep0_state {
	DWC2_EP0_SETUP,
	DWC2_EP0_DATA_IN,
	DWC2_EP0_DATA_OUT,
	DWC2_EP0_STATUS_IN,
	DWC2_EP0_STATUS_OUT,
};

enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,		
	USB_SPEED_LOW, USB_SPEED_FULL,		
	USB_SPEED_HIGH,				
	USB_SPEED_WIRELESS,			
	USB_SPEED_SUPER,		
	USB_SPEED_SUPER_PLUS,			
};

enum usb_device_state {
	USB_STATE_NOTATTACHED = 0,
	USB_STATE_ATTACHED,
	USB_STATE_DEFAULT,			
	USB_STATE_ADDRESS,
	USB_STATE_CONFIGURED,
	USB_STATE_CDC_ENUMERATE,
};


#define	EOPNOTSUPP	95
#define	EINPROGRESS	115	

#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
#define USB_DT_BOS				0x0f

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK			(0x03 << 5)
#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

/*
 * USB recipients, the third of three bRequestType fields
 */
#define USB_RECIP_MASK			0x1f
#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03

#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B


#define USB_DT_DEVICE_SIZE		18
#define USB_DT_QUALIFIER_SIZE		10
#define USB_DT_CONFIG_SIZE		9
#define USB_DT_INTERFACE_SIZE		9
#define USB_DT_ENDPOINT_SIZE		7
/*add by junbao*/
#define USB_DT_ASSOCIATION_SIZE		8
#define USB_DT_CDC_SIZE				5
#define USB_DT_ACM_SIZE				4
#define USB_DT_CONFIGS_SIZE		(USB_DT_CONFIG_SIZE + USB_DT_ASSOCIATION_SIZE + USB_DT_INTERFACE_SIZE +                     \
									USB_DT_CDC_SIZE + USB_DT_CDC_SIZE + USB_DT_ACM_SIZE + USB_DT_CDC_SIZE + 				\
									USB_DT_ENDPOINT_SIZE + USB_DT_INTERFACE_SIZE + USB_DT_ENDPOINT_SIZE + USB_DT_ENDPOINT_SIZE)

//#define USB_DT_CONFIGS_SIZE		(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE + USB_DT_ENDPOINT_SIZE + USB_DT_ENDPOINT_SIZE)
#define USB_DT_BOS_SIZE			5
#define USB_DT_USB_EXT_CAP_SIZE			7
#define USB_DT_BOS_CONFIG_SIZE	(USB_DT_BOS_SIZE + USB_DT_USB_EXT_CAP_SIZE)

#define USB_DT_TYPE_STRING_SIZE			4
#define USB_DT_TYPE_STRING1_SIZE		0x42
#define USB_DT_TYPE_STRING2_SIZE		0x26

#define USB_DT_CDC_LINE_CODING_SIZE			7


/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor {
	u8  bLength;
	u8  bDescriptorType;

	u16 bcdUSB;
	u8  bDeviceClass;
	u8  bDeviceSubClass;
	u8  bDeviceProtocol;
	u8  bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8  iManufacturer;
	u8  iProduct;
	u8  iSerialNumber;
	u8  bNumConfigurations;
};

struct usb_ctrlrequest {
	u8 bRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
};

struct usb_req {
	void		*buf;
	u32			length;
	u32			actual;
	int 		status;
	u32			zero;
	u32			complete;
};

struct dwc2_ep {
	unsigned int		index;
	unsigned int		size_loaded;
	unsigned int		last_load;
	unsigned int		maxpacket;
	unsigned int		maxp_xfersize;
	unsigned int		dir_in;
	unsigned int		send_zlp;	
};
void usb_vpid(void);
void dwc2_init(void);
bool usb_enumerate_device(void);
u32 usb_xfr_data(u8 *xfr, u32 size);
u32 usb_get_data(u8 *rcv,u32 size);
void dwc2_sw_init(void);


#endif 

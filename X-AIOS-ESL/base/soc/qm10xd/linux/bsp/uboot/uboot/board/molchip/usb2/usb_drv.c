/*
 * pcd.c
 *
 *  Created on: 2018
 *      Author: jason.qin
 */
#include <common.h> 
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/usb_hw.h>
#include <mach/usb_drv.h>
#include <mach/systimer.h>
#include <mach/uart_drv.h>

#define usb_min(a, b) (((a) < (b)) ? (a) : (b))

#define USB2_APB_CFG_REG	0x0B600000
#define USB2_APB_VBUS_REG	0x0B60001C

#define REG_USB_CFG_BASE REG_USB0_CFG_BASE

#define EP0_DATA_SIZE	128
#define EP1_BULKIN_DATA_SIZE 512
#define EP1_BULKOUT_DATA_SIZE 1024

enum usb_device_speed dev_speed = USB_SPEED_UNKNOWN;
enum usb_device_state dev_status = USB_STATE_NOTATTACHED;
enum dwc2_ep0_state ep0_state;
u8 setup_pack[8] __attribute__((aligned(4)));

u8 eps0_out_buf[EP0_DATA_SIZE] __attribute__((aligned(4)));   //ep0  out data buffer
u8 eps0_in_buf[EP0_DATA_SIZE] __attribute__((aligned(4)));    ////ep0 in data buffer
u8 ep1_in_buf[EP1_BULKIN_DATA_SIZE] __attribute__((aligned(4)));   //ep1 bulk-in buffer
u8 ep1_out_buf[EP1_BULKOUT_DATA_SIZE] __attribute__((aligned(4)));  //ep1 bulk-out buffer


struct dwc2_ep eps_in __attribute__((aligned(4)));    		//ep1 bulk in ep struct
struct dwc2_ep eps_out __attribute__((aligned(4)));	  		//ep1 bulk out ep struct
struct dwc2_ep eps_0 __attribute__((aligned(4)));    		//ep0 control dual direction ep struct
struct usb_req eps_out_req[2] __attribute__((aligned(4)));  //eps_out_req[0]£ºep0 out data req  eps_out_req[1]£ºep1 bulk out req 
struct usb_req eps_in_req[2] __attribute__((aligned(4)));   //eps_in_req[0]£ºep0 in data req  eps_in_req[1]£ºep1 bulk in req
struct usb_req eps_ctrl_req __attribute__((aligned(4)));

u8 d_desc[USB_DT_DEVICE_SIZE] __attribute__((aligned(4))) = 
	{USB_DT_DEVICE_SIZE,0x01,0x00,0x02,0x02,0x00,0x00,
	0x40,0x25,0x05,0xa7,0xa4,0x09,0x04,0x01,0x02,0x00,0x01};
u8 const q_desc[USB_DT_QUALIFIER_SIZE] __attribute__((aligned(4)))= 
	{USB_DT_QUALIFIER_SIZE,0x06,0x00,0x02,0x00,0x02,0x00,0x40,0x01,0x00};
u8 const c_desc[USB_DT_CONFIG_SIZE] __attribute__((aligned(4)))= 
	{USB_DT_INTERFACE_SIZE,0x02,0x020,0x00,0x01,0x01,0x00,0xe0,0x00};
/*u8 const cs_hs_desc[USB_DT_CONFIGS_SIZE] __attribute__((aligned(4)))= 
	{USB_DT_INTERFACE_SIZE,0x02,0x020,0x00,0x01,0x01,0x00,0xe0,0x00,
	USB_DT_INTERFACE_SIZE,0x04,0x00,0x00,0x02,0x0a,0x00,0x00,0x00,
	USB_DT_ENDPOINT_SIZE,0x05,0x081,0x02,0x00,0x02,0x00,
	USB_DT_ENDPOINT_SIZE,0x05,0x001,0x02,0x00,0x02,0x00};*/

u8 const cs_hs_desc[USB_DT_CONFIGS_SIZE] __attribute__((aligned(4)))= 
	{USB_DT_INTERFACE_SIZE,0x02,0x4b,0x00,0x02,0x02,0x04,0xc0,0x01,
	USB_DT_ASSOCIATION_SIZE,0x0b,0x00,0x02,0x02,0x02,0x01,0x07,
	USB_DT_INTERFACE_SIZE,0x04,0x00,0x00,0x01,0x02,0x02,0x01,0x05,
	USB_DT_CDC_SIZE,0x24,0x00,0x10,0x01,
	USB_DT_CDC_SIZE,0x24,0x01,0x00,0x01,
	USB_DT_ACM_SIZE,0x24,0x02,0x02,
	USB_DT_CDC_SIZE,0x24,0x06,0x00,0x01,
	USB_DT_ENDPOINT_SIZE,0x05,0x82,0x03,0x0a,0x00,0x09,
	USB_DT_INTERFACE_SIZE,0x04,0x01,0x00,0x02,0x0a,0x00,0x00,0x06,
	USB_DT_ENDPOINT_SIZE,0x05,0x81,0x02,0x00,0x02,0x00,
	USB_DT_ENDPOINT_SIZE,0x05,0x01,0x02,0x00,0x02,0x00};


u8 const bos_desc[USB_DT_BOS_CONFIG_SIZE] __attribute__((aligned(4))) = 
		{USB_DT_BOS_SIZE,0x0F,0x0C,0x00,0x01,
		 USB_DT_USB_EXT_CAP_SIZE,0x10,0x02,0x00,0x00,0x00,0x00};

u8 const cs_fs_desc[USB_DT_CONFIGS_SIZE] __attribute__((aligned(4)))= 
	{USB_DT_INTERFACE_SIZE,0x02,0x4b,0x00,0x02,0x02,0x04,0xc0,0x01,
	USB_DT_ASSOCIATION_SIZE,0x0b,0x00,0x02,0x02,0x02,0x01,0x07,
	USB_DT_INTERFACE_SIZE,0x04,0x00,0x00,0x01,0x02,0x02,0x01,0x05,
	USB_DT_CDC_SIZE,0x24,0x00,0x10,0x01,
	USB_DT_CDC_SIZE,0x24,0x01,0x00,0x01,
	USB_DT_ACM_SIZE,0x24,0x02,0x02,
	USB_DT_CDC_SIZE,0x24,0x06,0x00,0x01,
	USB_DT_ENDPOINT_SIZE,0x05,0x82,0x03,0x0a,0x00,0x09,
	USB_DT_INTERFACE_SIZE,0x04,0x01,0x00,0x02,0x0a,0x00,0x00,0x06,
	USB_DT_ENDPOINT_SIZE,0x05,0x81,0x02,0x40,0x00,0x00,
	USB_DT_ENDPOINT_SIZE,0x05,0x01,0x02,0x40,0x00,0x00};

u8 const Line_Coding[USB_DT_CDC_LINE_CODING_SIZE] __attribute__((aligned(4)))={0x00,0x10,0x0e,0x00,0x00,0x00,0x08};

u8 const device_type_string[USB_DT_TYPE_STRING_SIZE]  __attribute__((aligned(4))) = {
	USB_DT_TYPE_STRING_SIZE,3,9,4,
};

u8 const device_string_index1[USB_DT_TYPE_STRING1_SIZE]  __attribute__((aligned(4))) = {
	USB_DT_TYPE_STRING1_SIZE, 0x03, 0x4C, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x75, 0x00, 0x78, 0x00, 0x20, 0x00, 0x34, 0x00, 0x2E, 0x00, 0x39, 0x00, 0x2E, 0x00, 0x31, 0x00, 0x33, 0x00, 0x38, 0x00, 0x20, 0x00, 0x77, 0x00, 0x69, 
	0x00, 0x74, 0x00, 0x68, 0x00, 0x20, 0x00, 0x31, 0x00, 0x30, 0x00, 0x64, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x2E, 0x00, 0x64, 0x00, 0x77, 0x00, 0x63, 0x00, 0x32, 0x00,
};

u8 const device_string_index2[USB_DT_TYPE_STRING2_SIZE]  __attribute__((aligned(4))) = {
	USB_DT_TYPE_STRING2_SIZE, 0x03, 0x47, 0x00, 0x61, 0x00, 0x64, 0x00, 0x67, 0x00, 0x65, 0x00, 0x74, 0x00, 0x20, 0x00,
	0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x20, 0x00, 0x76, 0x00,
	0x32, 0x00, 0x2E, 0x00, 0x34, 0x00,
};

void usb_vpid(void)
{
/*	bool ret;
	u32 idvendor;
	u32 idproduct;
	struct usb_device_descriptor *desc;
	if(usb_id_enalbe()){
		desc = (struct usb_device_descriptor *)d_desc;
		//read idvendor  efsue block 12 high 16 bits
		ret = read_efuse_entry(&idvendor, 1, USB_IDVENDOR_BONDING_OPT);
		if ((ret == false)) {
			return;
		}
		//read idproduct efsue block 13 low 16 bits
		ret = read_efuse_entry(&idproduct, 1, USB_IDPRODUCT_BONDING_OPT);
		if ((ret == false)) {
			return;
		}

		//both read success ,update to device secriptor
		//idvendor high 16bits
		desc->idVendor = (u16)((idvendor >> 16) & 0xffff);
		
		//idproduct low 16bits
		desc->idProduct = (u16)(idproduct & 0xffff);
	}*/

	return;
}
static u32 dwc2_ep0_mps(u32 mps)
{
	switch (mps) {
	case 64:
		return D0EPCTL_MPS_64;
	case 32:
		return D0EPCTL_MPS_32;
	case 16:
		return D0EPCTL_MPS_16;
	case 8:
	default:
		return D0EPCTL_MPS_8;
	}
}

void dwc2_enqueue_setup(void);

void dwc2_start_req(struct dwc2_ep *ep, struct usb_req *req, bool continuing)
{
	u32 epctrl_reg;
	u32 epsize_reg;
	u32 epsize;
	u32 ctrl;
	unsigned int length;
	unsigned int packets;
	unsigned int maxreq;
	unsigned int dma_reg;

	if (!continuing) {
		req->complete = 0;
		req->status = -EINPROGRESS;
		req->actual = 0;
	}

	dma_reg = ep->dir_in ? DIEPDMA(ep->index) : DOEPDMA(ep->index);
	epctrl_reg = ep->dir_in ? DIEPCTL(ep->index) : DOEPCTL(ep->index);
	epsize_reg = ep->dir_in ? DIEPTSIZ(ep->index) : DOEPTSIZ(ep->index);
	
	ctrl = __raw_readl(REG_USB_CFG_BASE + epctrl_reg);

	if (ep->index && ctrl & DXEPCTL_STALL) {
		return;
	}
	
	length = req->length - req->actual;
	maxreq = ep->maxp_xfersize;
		
	if (length > maxreq) {
		int round = maxreq % ep->maxpacket;
		if (round)
			maxreq -= round;
		length = maxreq;
	}

	if (length)
		packets = DIV_ROUND_UP(length, ep->maxpacket);
	else
		packets = 1;	/* send one packet if length is zero. */

	if (ep->dir_in && ep->index != 0)
		epsize = DXEPTSIZ_MC(1);
	else
		epsize = 0;

	if (ep->dir_in && req->zero && !continuing) {
		/* Test if zlp is actually required. */
		if ((req->length >= ep->maxpacket) &&
		    !(req->length % ep->maxpacket))
			ep->send_zlp = 1;
	}

	epsize |= DXEPTSIZ_PKTCNT(packets);
	epsize |= DXEPTSIZ_XFERSIZE(length);

	/* write size / packets */
	__raw_writel(epsize, REG_USB_CFG_BASE + epsize_reg);

	if ((length != 0)  && !continuing) {
		__raw_writel((u32)req->buf, REG_USB_CFG_BASE + dma_reg);
	}
	ctrl |= DXEPCTL_EPENA;	/* ensure ep enabled */

	/* For Setup request do not clear NAK */
	if (!(ep->index == 0 && ep0_state == DWC2_EP0_SETUP)) {
		/* clear NAK set by core */
		ctrl |= DXEPCTL_CNAK;	
	}
	__raw_writel(ctrl, REG_USB_CFG_BASE + epctrl_reg);

	ep->size_loaded = length;
	ep->last_load = req->actual;		 
}

void dwc2_program_zlp(struct dwc2_ep *hs_ep)
{
	u32 ctrl;
	u8 index = hs_ep->index;
	u32 epctl_reg = hs_ep->dir_in ? DIEPCTL(index) : DOEPCTL(index);
	u32 epsiz_reg = hs_ep->dir_in ? DIEPTSIZ(index) : DOEPTSIZ(index);

	__raw_writel(DXEPTSIZ_MC(1) | DXEPTSIZ_PKTCNT(1) |
		    DXEPTSIZ_XFERSIZE(0), REG_USB_CFG_BASE +
		    epsiz_reg);

	ctrl = __raw_readl(REG_USB_CFG_BASE + epctl_reg);
	ctrl |= DXEPCTL_CNAK;  /* clear NAK set by core */
	ctrl |= DXEPCTL_EPENA; /* ensure ep enabled */
	ctrl |= DXEPCTL_USBACTEP;
	__raw_writel(ctrl, REG_USB_CFG_BASE + epctl_reg);
}

void dwc2_ep_enable(unsigned int index, bool dir_in)
{
	u32 epctrl_reg, epctrl, mps;
	
	if (dev_speed == USB_SPEED_HIGH)
		mps = 0x200;
	else
		mps = 0x40;

	/* note, we handle this here instead of dwc2_hsotg_set_ep_maxpacket */
	epctrl_reg = dir_in ? DIEPCTL(index) : DOEPCTL(index);
	epctrl = __raw_readl(REG_USB_CFG_BASE + epctrl_reg);
	epctrl &= ~(DXEPCTL_EPTYPE_MASK | DXEPCTL_MPS_MASK);
	epctrl |= mps;
	epctrl |= DXEPCTL_USBACTEP;
	epctrl |= DXEPCTL_EPTYPE_BULK;
	epctrl |= DXEPCTL_SETD0PID;
	if (dir_in) {
		epctrl &= ~DXEPCTL_TXFNUM_MASK;
		epctrl |= DXEPCTL_TXFNUM(index);
	}
	__raw_writel(epctrl, REG_USB_CFG_BASE + epctrl_reg);
}

int dwc2_setup_process(const struct usb_ctrlrequest *ctrl)
{
	struct usb_req *req = eps_0.dir_in ? &eps_in_req[0] :&eps_out_req[0];
	int				value = -EOPNOTSUPP;
	u16				w_value = ctrl->wValue;
	u16				w_length = ctrl->wLength;

	req->zero = 0;
	req->length = 0;
	req->complete = 0;
	req->status = -EINPROGRESS;

	if ((ctrl->bRequestType & USB_TYPE_MASK) != USB_TYPE_STANDARD)
		goto unknown;

	switch (ctrl->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		if (ctrl->bRequestType != USB_DIR_IN)
			goto unknown;
		switch (w_value >> 8) {
		case USB_DT_DEVICE:
			value = usb_min(w_length, USB_DT_DEVICE_SIZE);
			memcpy(req->buf, d_desc, value);
			break;
		case USB_DT_DEVICE_QUALIFIER:
			if (dev_speed >= USB_SPEED_HIGH)
				break;
			value = usb_min(w_length, USB_DT_QUALIFIER_SIZE);
			memcpy(req->buf, q_desc, value);
			break;
		case USB_DT_CONFIG:			
			value = usb_min(w_length, USB_DT_CONFIGS_SIZE);
			if (dev_speed == USB_SPEED_HIGH)
				memcpy(req->buf, cs_hs_desc,value);
			else
				memcpy(req->buf, cs_fs_desc, value);
			break;
		case USB_DT_STRING:
			if(0 == (w_value & 0xff)){  //descriptor index 0
				value = usb_min(w_length, USB_DT_TYPE_STRING_SIZE);
				memcpy(req->buf, device_type_string, value);
			} else if(1 == (w_value & 0xff)){ //descriptor index 1
				value = usb_min(w_length,USB_DT_TYPE_STRING1_SIZE);
				memcpy(req->buf, device_string_index1, value);
			}else{     //descriptor index 2
				value = usb_min(w_length,USB_DT_TYPE_STRING2_SIZE);
				memcpy(req->buf, device_string_index2, value);
			}
			break;
		case USB_DT_BOS:
			value = usb_min(w_length, USB_DT_BOS_CONFIG_SIZE);
			memcpy(req->buf, bos_desc, value);
			break;
		}
		break;

	case USB_REQ_SET_CONFIGURATION:
		if (ctrl->bRequestType != 0)
			goto unknown;
		/* Enabel ep1in and ep1out INT*/
		__raw_writel(__raw_readl(REG_USB_CFG_BASE + DAINTMSK) | 
			DAINT_INEP(1) | DAINT_OUTEP(1), REG_USB_CFG_BASE + DAINTMSK);
		dev_status = USB_STATE_CONFIGURED;
		uart_puts("USC\r\n");
		value = 0;
		break;

	/* function drivers must handle get/set altsetting */
	case USB_REQ_SET_INTERFACE:
		value = 0;
		break;
	case USB_REQ_GET_INTERFACE:
		*((u8 *)req->buf) = 0;
		value = usb_min(w_length, (u16) 1);
		break;
	case USB_REQ_GET_STATUS:
		*((u16 *)(req->buf)) = 0;
		value = usb_min(w_length, (u16) 1);
		break;

	case USB_REQ_CLEAR_FEATURE:
	case USB_REQ_SET_FEATURE:
		value = 0;
		break;
	default:
unknown:
		if (ctrl->bRequestType & USB_DIR_IN) {
			switch (ctrl->bRequestType & USB_RECIP_MASK) {
			case USB_RECIP_DEVICE:
				break;
			
			case USB_RECIP_INTERFACE:
				switch (ctrl->bRequest) {
				case 0x21:
					value = usb_min(w_length,USB_DT_CDC_LINE_CODING_SIZE);
					memcpy(req->buf, Line_Coding, value);
				break;
				}
			break;

			case USB_RECIP_ENDPOINT:
			break;

			default:
				return -EOPNOTSUPP;
			}
		} else {
			switch (ctrl->bRequestType & USB_RECIP_MASK) {
			case USB_RECIP_DEVICE:
				value = 0;
				break;
			case USB_RECIP_INTERFACE:
				switch (ctrl->bRequest) {
				case 0x22:
					if(dev_status == USB_STATE_CONFIGURED) {
						dev_status = USB_STATE_CDC_ENUMERATE;	
						dwc2_ep_enable(1, out);
						dwc2_ep_enable(1, in);
						eps_in_req[1].buf = (void *)ep1_in_buf;

						eps_out_req[1].length = EP1_BULKOUT_DATA_SIZE;
						eps_out_req[1].buf = (void *)ep1_out_buf;
						eps_out_req[1].zero = 0;

						dwc2_start_req(&eps_out, &eps_out_req[1], false);
						uart_puts("UPO\r\n");
					}
					value = 0;
					break;
				
				case 0x20:
					value = w_length;
					break;
				}		
				
			break;
			case USB_RECIP_ENDPOINT:
			break;

			default:
				return -EOPNOTSUPP;
			}		
		}
	}
	if (value >= 0) {
		req->length = value;
		req->zero = value < w_length;
		dwc2_start_req(&eps_0, req, false);
	}

	return value;
}


void dwc2_process_control(struct usb_ctrlrequest *ctrl)
{
	struct usb_req *req;
	int ret = 0;
	u32 reg;

	if (ctrl->wLength == 0) {
		eps_0.dir_in = 1;
		ep0_state = DWC2_EP0_STATUS_IN;
	} else if (ctrl->bRequestType & USB_DIR_IN) {
		eps_0.dir_in = 1;
		ep0_state = DWC2_EP0_DATA_IN;				
	} else {
		eps_0.dir_in = 0;
		ep0_state = DWC2_EP0_DATA_OUT;			
	}
	

	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		switch (ctrl->bRequest) {
		case USB_REQ_SET_ADDRESS:
			dev_status = USB_STATE_ADDRESS;
			reg = __raw_readl(REG_USB_CFG_BASE + DCFG);
			reg &= ~DCFG_DEVADDR_MASK;
			reg |= (ctrl->wValue <<
				 DCFG_DEVADDR_SHIFT) & DCFG_DEVADDR_MASK;
			__raw_writel(reg, REG_USB_CFG_BASE + DCFG);
			req = eps_0.dir_in ? &eps_in_req[0] : &eps_out_req[0];
			req->length = 0;
			req->zero = 0;
			dwc2_start_req(&eps_0, req, false);
			return;

		case USB_REQ_GET_STATUS:
			req = eps_0.dir_in ? &eps_in_req[0] : &eps_out_req[0];
			*(u16 *)req->buf = 0;
			req->length = 2;
			req->zero = 0;
			dwc2_start_req(&eps_0, req, false);
			return;

		case USB_REQ_CLEAR_FEATURE:
		case USB_REQ_SET_FEATURE:
			req = eps_0.dir_in ? &eps_in_req[0] : &eps_out_req[0];
			req->length = 0;
			req->zero = 0;
			dwc2_start_req(&eps_0, req, false);
			return;
		}
	}

	if (ret == 0)
		ret = dwc2_setup_process(ctrl);

	if (ret < 0) {
		reg = __raw_readl(REG_USB_CFG_BASE + (eps_0.dir_in) ? DIEPCTL0 : DOEPCTL0);
		reg |= DXEPCTL_STALL;
		reg |= DXEPCTL_CNAK;
		__raw_writel(reg, REG_USB_CFG_BASE + (eps_0.dir_in) ? DIEPCTL0 : DOEPCTL0);
		dwc2_enqueue_setup();
	}
}

void dwc2_irq_enumdone(void)
{
	u32 reg = __raw_readl(REG_USB_CFG_BASE + DSTS);
	int ep0_mps = 0, ep_mps = 8;

	dev_status = USB_STATE_DEFAULT;
	switch ((reg & DSTS_ENUMSPD_MASK) >> DSTS_ENUMSPD_SHIFT) {
	case DSTS_ENUMSPD_FS:
	case DSTS_ENUMSPD_FS48:
		dev_speed = USB_SPEED_FULL;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 64;
		break;

	case DSTS_ENUMSPD_HS:
		dev_speed = USB_SPEED_HIGH;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 512;
		break;
	}
	if (ep0_mps) {
		eps_0.maxpacket = ep0_mps;
		eps_0.maxp_xfersize = 2*ep0_mps;
		eps_0.index = 0;

		eps_in.maxpacket = ep_mps;
		eps_in.maxp_xfersize = ep_mps * DXEPTSIZ_PKTCNT_LIMIT;
		eps_in.dir_in = in;
		eps_in.index = 1;
	
		eps_out.maxpacket = ep_mps;
		eps_out.maxp_xfersize = ep_mps * DXEPTSIZ_PKTCNT_LIMIT;
		eps_out.dir_in = out;
		eps_out.index = 1;

		reg = __raw_readl(REG_USB_CFG_BASE + DIEPCTL(0));
		reg &= ~DXEPCTL_MPS_MASK;
		reg |= dwc2_ep0_mps(ep0_mps);
		__raw_writel(reg, REG_USB_CFG_BASE + DIEPCTL(0));
		reg = __raw_readl(REG_USB_CFG_BASE + DOEPCTL(0));
		reg &= ~DXEPCTL_MPS_MASK;
		reg |= dwc2_ep0_mps(ep0_mps);
		__raw_writel(reg, REG_USB_CFG_BASE + DOEPCTL(0));

		reg = __raw_readl(REG_USB_CFG_BASE + DIEPCTL(1));
		reg &= ~DXEPCTL_MPS_MASK;
		reg |= ep_mps;
		__raw_writel(reg, REG_USB_CFG_BASE + DIEPCTL(1));	
		reg = __raw_readl(REG_USB_CFG_BASE + DOEPCTL(1));
		reg &= ~DXEPCTL_MPS_MASK;
		reg |= ep_mps;
		__raw_writel(reg, REG_USB_CFG_BASE + DOEPCTL(1));	
	}

	/* ensure after enumeration our EP0 is active */
	dwc2_enqueue_setup();
}

void dwc2_complete_request(struct usb_req *req, int result)
{	
	if(req->status== -EINPROGRESS)
		req->status = result;
	req->complete = 1;
}


void dwc2_enqueue_setup(void)
{
	eps_ctrl_req.buf = (void *)setup_pack;
	eps_ctrl_req.zero = 0;
	eps_ctrl_req.length = 8;
	eps_ctrl_req.status = -EINPROGRESS;

	eps_0.dir_in = 0;
	eps_0.send_zlp = 0;
	ep0_state = DWC2_EP0_SETUP;		
	//uart_puts("dwc2_enqueue_setup \r\n");
	dwc2_start_req(&eps_0, &eps_ctrl_req, false);
}

void dwc2_complete_in(u32 epnum)
{
	struct dwc2_ep *ep = (epnum == 0) ? &eps_0 : &eps_in;
	struct usb_req *req = &eps_in_req[epnum];
	u32 epsize = __raw_readl(REG_USB_CFG_BASE + DIEPTSIZ(epnum));
	int size_left, size_done;

	/* Finish ZLP handling for IN EP0 transactions */
	if (epnum == 0 && ep0_state == DWC2_EP0_STATUS_IN) {
		/*
		 * While send zlp for DWC2_EP0_STATUS_IN EP direction was
		 * changed to IN. Change back to complete OUT transfer request
		 */
		eps_0.dir_in = 0;
		dwc2_complete_request(req, 0);
		dwc2_enqueue_setup();
		return;
	}

	/*
	 * Calculate the size of the transfer by checking how much is left
	 * in the endpoint size register and then working it out from
	 * the amount we loaded for the transfer.
	 *
	 * We do this even for DMA, as the transfer may have incremented
	 * past the end of the buffer (DMA transfers are always 32bit
	 * aligned).
	 */

	size_left = DXEPTSIZ_XFERSIZE_GET(epsize);
	size_done = ep->size_loaded - size_left;
	size_done += ep->last_load;
	req->actual = size_done;

	if (!size_left && req->actual < req->length) {
		dwc2_start_req(ep, req, true);
		return;
	}

	/* Zlp for all endpoints, for ep0 only in DATA IN stage */
	if (ep->send_zlp) {
		dwc2_program_zlp(ep);
		ep->send_zlp = 0;
		/* transfer will be completed on next complete interrupt */
		return;
	}

	if (epnum == 0 && ep0_state == DWC2_EP0_DATA_IN) {
		/* Move to STATUS OUT */			
		eps_0.dir_in = out;
		ep0_state = DWC2_EP0_STATUS_OUT;
		dwc2_program_zlp(&eps_0);
		return;
	}

	dwc2_complete_request(req, 0);
}


void dwc2_handle_outdone(u32 epnum)
{
 	u32 epsize = __raw_readl(REG_USB_CFG_BASE + DOEPTSIZ(epnum));
 	struct dwc2_ep *hs_ep = (epnum == 0) ? &eps_0 : &eps_out;
	struct usb_req *req = &eps_out_req[epnum];
	unsigned int size_left = DXEPTSIZ_XFERSIZE_GET(epsize);
	int result = 0;
	unsigned int size_done;

	if ((epnum == 0) && (ep0_state == DWC2_EP0_SETUP))
		req = &eps_ctrl_req;
		
	if (epnum == 0 && ep0_state == DWC2_EP0_STATUS_OUT) {
		dwc2_complete_request(req, 0);
		dwc2_enqueue_setup();
		return;
	}

	size_done = hs_ep->size_loaded - size_left;
	size_done += hs_ep->last_load;
	req->actual = size_done;

	/* if there is more request to do, schedule new transfer */
	if (req->actual < req->length && size_left == 0) {
		dwc2_start_req(hs_ep, req, true);
		return;
	}

	/* DDMA IN status phase will start from StsPhseRcvd interrupt */
	if (epnum == 0 && ep0_state == DWC2_EP0_DATA_OUT) {
		/* Move to STATUS IN */				
		eps_0.dir_in = in;
		ep0_state = DWC2_EP0_STATUS_IN;
		dwc2_program_zlp(&eps_0);
		return;
	}

	dwc2_complete_request(req, result);
}


void dwc2_epint(unsigned int idx, bool dir_in)
{
	u32 epint_reg = dir_in ? DIEPINT(idx) : DOEPINT(idx);
	u32 ints;
	u32 epmsk_reg = dir_in ? DIEPMSK : DOEPMSK;
	u32 mask;

	mask = __raw_readl(REG_USB_CFG_BASE + epmsk_reg);
	mask |= DXEPINT_SETUP_RCVD;
	
	ints = __raw_readl(REG_USB_CFG_BASE + epint_reg);

	ints &= mask;
	/* Clear endpoint interrupts */
	__raw_writel(ints, REG_USB_CFG_BASE + epint_reg);

	if((idx == 0) && (ints & (DXEPINT_SETUP | DXEPINT_SETUP_RCVD)))
		ints &= ~DXEPINT_XFERCOMPL;

	if ((idx == 0) && (dir_in == out) && (ints & DXEPINT_SETUP)) { 
		dwc2_handle_outdone(0);
	} else if (ints & DXEPINT_XFERCOMPL) {
		if (dir_in == in) {
			dwc2_complete_in(idx);
		} else {
			dwc2_handle_outdone(idx);
		}
	}
	if (ints  & DXEPINT_AHBERR)
		uart_puts("UAHBE\r\n");
}


void dwc2_irq(void)
{
	u32 gintsts;

	gintsts = __raw_readl(REG_USB_CFG_BASE + GINTSTS);

	if (gintsts & GINTSTS_RESETDET) {
		__raw_writel(GINTSTS_RESETDET, REG_USB_CFG_BASE + GINTSTS);
		uart_puts("URD\r\n");
	}

	if (gintsts & GINTSTS_USBRST) {
		__raw_writel(GINTSTS_USBRST, REG_USB_CFG_BASE + GINTSTS);
		uart_puts("URS\r\n");
		/* Reset device address to zero */
		__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCFG) & 
			(~DCFG_DEVADDR_MASK), REG_USB_CFG_BASE + DCFG);
	}

	if (gintsts & GINTSTS_ENUMDONE) {
		__raw_writel(GINTSTS_ENUMDONE, REG_USB_CFG_BASE + GINTSTS);
		dwc2_irq_enumdone();
		uart_puts("UEM\r\n");
	}

	if (gintsts & (GINTSTS_OEPINT | GINTSTS_IEPINT)) {
		u32 daint = __raw_readl(REG_USB_CFG_BASE + DAINT);
		u32 daintmsk = __raw_readl(REG_USB_CFG_BASE + DAINTMSK);
		u32 daint_out, daint_in;
		int ep;

		daint &= daintmsk;
		daint_out = daint >> DAINT_OUTEP_SHIFT;
		daint_in = daint & ~(daint_out << DAINT_OUTEP_SHIFT);

		for (ep = 0; ep < 2 && daint_out; ep++, daint_out >>= 1) {
			if (daint_out & 1)
				dwc2_epint(ep, out);
		}

		for (ep = 0; ep < 2  && daint_in; ep++, daint_in >>= 1) {
			if (daint_in & 1)
				dwc2_epint(ep, in);
		}
	}
	
	return;
}

void dwc2_init(void)
{
	u32 reg;

	//step1:usb sys config
	/* clk enable */
	reg = __raw_readl(REG_CEN_PERI_CLK_CTRL);
	reg |= (BIT(0) | BIT(1));
	__raw_writel(reg, REG_CEN_PERI_CLK_CTRL);

	/* vbus cfg */
	reg = __raw_readl(USB2_APB_VBUS_REG);
	reg |= (BIT(2) | BIT(3));
	__raw_writel(reg, USB2_APB_VBUS_REG);

	/* disable all scan-downs */
	reg = __raw_readl(USB2_APB_CFG_REG);
	reg &= ~(BIT(3) | BIT(4));
	__raw_writel(reg, USB2_APB_CFG_REG);

	/* utmiotg  iddig */
	reg = __raw_readl(USB2_APB_CFG_REG);
	reg |= (BIT(5) | BIT(6));
	__raw_writel(reg, USB2_APB_CFG_REG);


	//setp2:usb phy config
	

	//step3: core init
	/* Core Soft Reset */
	reg = __raw_readl(REG_USB_CFG_BASE + GRSTCTL);
	reg |= GRSTCTL_CSFTRST;
	__raw_writel(reg, REG_USB_CFG_BASE + GRSTCTL);

	do {//wait for reset bit clear
		reg = __raw_readl(REG_USB_CFG_BASE + GRSTCTL);
	} while (reg & GRSTCTL_CSFTRST);

	do {//wait for AHB Idle
		reg = __raw_readl(REG_USB_CFG_BASE + GRSTCTL);
	} while ((reg & GRSTCTL_AHBIDLE) == 0);

	/* Force Dev */
	reg = __raw_readl(REG_USB_CFG_BASE + GUSBCFG);
	reg &= ~GUSBCFG_FORCEHOSTMODE;
	reg |= GUSBCFG_FORCEDEVMODE;
	__raw_writel(reg, REG_USB_CFG_BASE + GUSBCFG);
	
	do {//wait for current mode 
		reg = __raw_readl(REG_USB_CFG_BASE + GINTSTS);
	} while (reg & GINTSTS_CURMODE_HOST);
	
	reg = __raw_readl(REG_USB_CFG_BASE + GUSBCFG);
	reg &= ~(GUSBCFG_TOUTCAL(7) | GUSBCFG_PHYIF16 | 
		GUSBCFG_SRPCAP | GUSBCFG_HNPCAP | GUSBCFG_USBTRDTIM_MASK);
	reg |= GUSBCFG_TOUTCAL(7) | 
		((PHY_IF == GUSBCFG_PHYIF8 ? 9 : 5) << GUSBCFG_USBTRDTIM_SHIFT);
	__raw_writel(reg, REG_USB_CFG_BASE + GUSBCFG);

	/* Set RX/NPTX FIFO sizes */
	__raw_writel(0x23D, REG_USB_CFG_BASE + GRXFSIZ);
	__raw_writel(0x40023D, REG_USB_CFG_BASE + GNPTXFSIZ);
	/* Set Tx FIFO sizes */
	__raw_writel(0x100027D, REG_USB_CFG_BASE + DPTXFSIZN(1));
	__raw_writel(0x100037D, REG_USB_CFG_BASE + DPTXFSIZN(2));
	__raw_writel(0x10047D, REG_USB_CFG_BASE + DPTXFSIZN(3));
	__raw_writel(0x10048D, REG_USB_CFG_BASE + DPTXFSIZN(4));
	__raw_writel(0x10049D, REG_USB_CFG_BASE + DPTXFSIZN(5));
	__raw_writel(0x1004AD, REG_USB_CFG_BASE + DPTXFSIZN(6));
	__raw_writel(0x1004BD, REG_USB_CFG_BASE + DPTXFSIZN(7));
	__raw_writel(0x1004CD, REG_USB_CFG_BASE + DPTXFSIZN(8));
	__raw_writel(0x1004DD, REG_USB_CFG_BASE + DPTXFSIZN(9));
	__raw_writel(0x1004ED, REG_USB_CFG_BASE + DPTXFSIZN(10));
	__raw_writel(0x1004FD, REG_USB_CFG_BASE + DPTXFSIZN(11));
	__raw_writel(0x10050D, REG_USB_CFG_BASE + DPTXFSIZN(12));
	__raw_writel(0x10051D, REG_USB_CFG_BASE + DPTXFSIZN(13));
	__raw_writel(0x10052D, REG_USB_CFG_BASE + DPTXFSIZN(14));
	__raw_writel(0x10053D, REG_USB_CFG_BASE + DPTXFSIZN(15));
	
	/* Set total fifo size */
	__raw_writel(0x54D0580, REG_USB_CFG_BASE + GDFIFOCFG);
	
	/* Flush all the transmit FIFOs */
	__raw_writel(GRSTCTL_TXFNUM(0x10) | GRSTCTL_TXFFLSH | 
		GRSTCTL_RXFFLSH, REG_USB_CFG_BASE + GRSTCTL);

	/* wait until the fifos are both flushed */
	do {
		reg = __raw_readl(REG_USB_CFG_BASE + GRSTCTL);
	} while(reg & (GRSTCTL_TXFFLSH | GRSTCTL_RXFFLSH));
	
	/* Soft disconnect */
	__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCTL) | 
		DCTL_SFTDISCON, REG_USB_CFG_BASE + DCTL);
	/* Set Max speed to High Speed */
#ifdef FORCE_FS
	reg = DCFG_DEVSPD_FS;
#else
	reg = DCFG_DEVSPD_HS;
#endif	
	/* Set IN Endpoint Mismatch Count to 1 */
	reg |= DCFG_EPMISCNT(1);
	__raw_writel(reg, REG_USB_CFG_BASE + DCFG);

   /* Clear any pending OTG interrupts */
	__raw_writel(0xffffffff, REG_USB_CFG_BASE + GOTGINT);

   /* Clear any pending interrupts */
	__raw_writel(0xffffffff, REG_USB_CFG_BASE + GINTSTS);
	
   	reg = GINTSTS_ERLYSUSP | GINTSTS_USBRST | GINTSTS_RESETDET |
		GINTSTS_ENUMDONE | GINTSTS_USBSUSP | GINTSTS_WKUPINT;
	__raw_writel(reg, REG_USB_CFG_BASE + GINTMSK);
	
	 /* Enable DMA, Enable Global INT */
	__raw_writel(GAHBCFG_GLBL_INTR_EN | GAHBCFG_DMA_EN |
			   (GAHBCFG_HBSTLEN_INCR << GAHBCFG_HBSTLEN_SHIFT),
			   REG_USB_CFG_BASE + GAHBCFG);
			   
   /* Set Dev IN EP INT Mask */
	reg = DIEPMSK_EPDISBLDMSK | DIEPMSK_XFERCOMPLMSK |
		DIEPMSK_TIMEOUTMSK | DIEPMSK_AHBERRMSK;
	__raw_writel(reg, REG_USB_CFG_BASE + DIEPMSK);
	
	/* Set Dev OUT EP INT Mask */
	reg = DOEPMSK_XFERCOMPLMSK | DOEPMSK_STSPHSERCVDMSK | 
		DOEPMSK_EPDISBLDMSK | DOEPMSK_AHBERRMSK | DOEPMSK_SETUPMSK;	
	__raw_writel(reg, REG_USB_CFG_BASE + DOEPMSK);

	/* Disable All EPs interrupt*/
	__raw_writel(0, REG_USB_CFG_BASE + DAINTMSK);

	/* Enable in and out endpoint interrupts */
	reg = __raw_readl(REG_USB_CFG_BASE + GINTMSK);
	__raw_writel(reg | GINTSTS_OEPINT | GINTSTS_IEPINT, REG_USB_CFG_BASE + GINTMSK);

   /* Enable interrupts for EP0 in and out */
	__raw_writel(DAINT_INEP(0) | DAINT_OUTEP(0), REG_USB_CFG_BASE + DAINTMSK);

	__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCTL) | 
		DCTL_PWRONPRGDONE, REG_USB_CFG_BASE + DCTL);
	delayms(1);
	__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCTL) & 
		(~DCTL_PWRONPRGDONE), REG_USB_CFG_BASE + DCTL);

	/* Set EP OUT 0 to read 1 8byte packet */
	__raw_writel(DXEPTSIZ_MC(1) | DXEPTSIZ_PKTCNT(1) |
		DXEPTSIZ_XFERSIZE(8), REG_USB_CFG_BASE + DOEPTSIZ0);

	/* enable EP0OUT */
	__raw_writel(D0EPCTL_MPS_64 | DXEPCTL_CNAK | DXEPCTL_EPENA | DXEPCTL_USBACTEP,
		REG_USB_CFG_BASE + DOEPCTL0);

	/* enable EP0IN */
	__raw_writel(D0EPCTL_MPS_64 | DXEPCTL_USBACTEP, REG_USB_CFG_BASE + DIEPCTL0);

	/* clear global NAKs */
	reg = DCTL_CGOUTNAK | DCTL_CGNPINNAK;
	__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCTL) | reg, REG_USB_CFG_BASE + DCTL);

	/* must be at-least 3ms to allow bus to see disconnect */
	delayms(10);
	/* clear soft disconnect */
	__raw_writel(__raw_readl(REG_USB_CFG_BASE + DCTL) & 
		(~DCTL_SFTDISCON), REG_USB_CFG_BASE + DCTL);

	//step4: phy reset
	/* utmi rest */
	reg = __raw_readl(REG_AP_PERI_SOFT_RST0);
	reg |= BIT(29);
	__raw_writel(reg, REG_AP_PERI_SOFT_RST0);

	delayms(10);

	reg = __raw_readl(REG_AP_PERI_SOFT_RST0);
	reg &= ~(BIT(29));
	__raw_writel(reg, REG_AP_PERI_SOFT_RST0);


	//step5: setup sw buffer
	eps_ctrl_req.buf = (void *)setup_pack;
	eps_in_req[0].buf = (void *)eps0_in_buf;
	eps_out_req[0].buf = (void *)eps0_out_buf;

	eps_in_req[1].buf = (void *)ep1_in_buf;
	eps_out_req[1].buf = (void *)ep1_out_buf;
	eps_0.maxpacket = 64;
	eps_0.maxp_xfersize = 128;
	eps_0.index = 0;

	//step6: recv setup packet
	dwc2_enqueue_setup();

}

void dwc2_sw_init(void)
{
	u32 reg = __raw_readl(REG_USB_CFG_BASE + DSTS);
	int ep0_mps = 0, ep_mps = 8;

	switch ((reg & DSTS_ENUMSPD_MASK) >> DSTS_ENUMSPD_SHIFT) {
	case DSTS_ENUMSPD_FS:
	case DSTS_ENUMSPD_FS48:
		dev_speed = USB_SPEED_FULL;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 64;
		break;

	case DSTS_ENUMSPD_HS:
		dev_speed = USB_SPEED_HIGH;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 512;
		break;
	}
	if (ep0_mps) {
		eps_0.maxpacket = ep0_mps;
		eps_0.maxp_xfersize = 2*ep0_mps;
		eps_0.index = 0;

		eps_in.maxpacket = ep_mps;
		eps_in.maxp_xfersize = ep_mps * DXEPTSIZ_PKTCNT_LIMIT;
		eps_in.dir_in = in;
		eps_in.index = 1;
	
		eps_out.maxpacket = ep_mps;
		eps_out.maxp_xfersize = ep_mps * DXEPTSIZ_PKTCNT_LIMIT;
		eps_out.dir_in = out;
		eps_out.index = 1;
	}

	eps_ctrl_req.buf = (void *)setup_pack;
	eps_in_req[0].buf = (void *)eps0_in_buf;
	eps_out_req[0].buf = (void *)eps0_out_buf;

	eps_in_req[1].buf = (void *)ep1_in_buf;
	eps_out_req[1].buf = (void *)ep1_out_buf;
	eps_out_req[1].length = EP1_BULKOUT_DATA_SIZE;
	eps_out_req[1].zero = 0;

#if 0
	//high speed packet size control ep:64 bulk ep:512
	eps_0.maxpacket = 64;
	eps_0.maxp_xfersize = 128;
	eps_0.index = 0;

	eps_in.maxpacket = 512;
	eps_in.maxp_xfersize = 512 * DXEPTSIZ_PKTCNT_LIMIT;
	eps_in.dir_in = in;
	eps_in.index = 1;

	eps_out.maxpacket = 512;
	eps_out.maxp_xfersize = 512 * DXEPTSIZ_PKTCNT_LIMIT;
	eps_out.dir_in = out;
	eps_out.index = 1;
#endif

	dwc2_enqueue_setup();

	dwc2_start_req(&eps_out, &eps_out_req[1], false);

}

bool usb_enumerate_device(void)
{
	bool ret = true;

	do{
		dwc2_irq();
		if (eps_ctrl_req.complete) {
			eps_ctrl_req.complete = 0;
			if (eps_ctrl_req.actual) {
				dwc2_process_control(eps_ctrl_req.buf);
			} else{
				dwc2_enqueue_setup();
			}
		}
	}while(dev_status != USB_STATE_CDC_ENUMERATE);

	return ret;
}

/*bulk in*/
/*size - xfr buffer size
 *
 *return -  0:err  others:actual transfer size
*/
u32 usb_xfr_data(u8 *xfr, u32 size)
{
	u32 len = 0;

	if(size > EP1_BULKIN_DATA_SIZE)
		return len;

	//start xfr
	eps_in_req[1].buf = (void *)ep1_in_buf;
	memcpy(eps_in_req[1].buf,xfr,size);
	eps_in_req[1].length = size;
	eps_in_req[1].zero = 0;
	dwc2_start_req(&eps_in, &eps_in_req[1], false);

	//poll interrupts for waiting done
	while(1){
		dwc2_irq();
		/*if recive any control setup packet ,deal with it*/
		if (eps_ctrl_req.complete) {
			eps_ctrl_req.complete = 0;
			//uart_puts("eps_ctrl_req.complete \r\n");
			if (eps_ctrl_req.actual) {
				dwc2_process_control(eps_ctrl_req.buf);
			} else{
				dwc2_enqueue_setup();
			}
		}

		if (eps_in_req[1].complete) {
			len = eps_in_req[1].actual;
			eps_in_req[1].complete = 0;
			break;
		}

	}
	
	return len;

}
/*bulk out*/
/*size - rcv buffer size
 *
 *return -  0:err  others:actual transfer size
*/
u32 usb_get_data(u8 *rcv,u32 size)
{
	u32 len = 0;

	if(size > EP1_BULKOUT_DATA_SIZE)
		return 0;
	
	while(1){
		dwc2_irq();
		if (eps_ctrl_req.complete) {
			eps_ctrl_req.complete = 0;
			//uart_puts("eps_ctrl_req.complete \r\n");
			if (eps_ctrl_req.actual) {
				dwc2_process_control(eps_ctrl_req.buf);
			} else{
				dwc2_enqueue_setup();
			}
		}

		if (eps_out_req[1].complete) {
			eps_out_req[1].complete = 0;
			len = usb_min(size, eps_out_req[1].actual);
			memcpy(rcv,eps_out_req[1].buf,len);
			//start new bulk out request for data
			dwc2_start_req(&eps_out, &eps_out_req[1], false);
			break;
		}
		
	}

	return len;
}




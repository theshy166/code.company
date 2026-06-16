UVC 功能
1. kernel 配置文件中开启USB功能后，还必须增加如下配置

   CONFIG_MEDIA_SUPPORT=y
   #
   # Multimedia core support
   #
   CONFIG_MEDIA_CAMERA_SUPPORT=y
   CONFIG_VIDEO_DEV=y
   CONFIG_VIDEO_V4L2=y
   CONFIG_VIDEOBUF2_CORE=y
   
   #
   # Media drivers
   #
   CONFIG_MEDIA_USB_SUPPORT=y
   #
   # Webcam devices
   #
   CONFIG_USB_VIDEO_CLASS=y
   
   #
   # Gadget/Dual-role mode requires USB Gadget support to be enabled
   #
   CONFIG_USB_DWC2_PERIPHERAL=y
   
   CONFIG_USB_GADGET=y
   
   # CONFIG_USB_GADGET_VERBOSE is not set
   #
   # USB Peripheral Controller
   #
   CONFIG_USB_LIBCOMPOSITE=y
   CONFIG_USB_F_UVC=y
   CONFIG_USB_CONFIGFS_F_UVC=y
   CONFIG_USB_G_WEBCAM=y
   CONFIG_UVC_HIGH_BANDWIDTH=y
   CONFIG_UVC_INS_UAC=y
   CONFIG_ENABLE_SPEAKER_DESC=y
   
2. API调用接口，库文件名称libquamedia.uvc.so
   quauvc_start()
   quauvc_exit()
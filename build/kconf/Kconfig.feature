#
# For a description of the syntax of this configuration file,
# feature configuration
#

menu "Feature"

config XOS_FB_ROTATION
	bool "fb_rotation"
	default n
	help
		Select fb rotation.

config XOS_FB_USE_ARGB1555
	bool "fb format use ARGB1555"
	default n
	help
		Select fb format.

config XOS_USE_PHYSICAL_KEY
	bool "physical key"
	default n
	help
		Select the physical key.

config XOS_USE_BUTTOM_BACK
	bool "buttom back"
	default n
	help
		Select the buttom back.
		
config XOS_USE_TOUCH_PANEL
	bool "touch panel"
	default n
	help
		Select the touch panel.
                
config XOS_VOICE_INTERACTION
	bool "voice_interaction"
	default n
	help
		Select the app voice interaction.

config XOS_USE_UVC
	bool "uvc"
	default n
	help
		Select the app uvc.

config XOS_USE_QUAAI_CLASSIFY
	bool "quaai classify"
	default n
	help
		Select the quaai classify model.

config XOS_USE_QUAAI_GESTURE
	bool "quaai gesture"
	default n
	help
		Select the quaai gesture model.

config XOS_USE_QUAAI_POSE
	bool "quaai pose analysis"
	default n
	help
		Select the quaai pose model.

config XOS_HW_BLEND_COLOR
	bool "hw blend color"
	default n
	help
		Select the hw blend color.

config XOS_USE_LIB_JPEG
	bool "lvgl enable libjpeg"
	default n
	help
		Select the libjpeg.

config XOS_USE_HW_JPEG
	bool "lvgl enable qua hw jpeg"
	default n
	help
		Select the hw jpeg.

#add more external features here
#
endmenu



#os types select
#
menu "OS Configure"

#comment ""
#comment "|---------------------------------------------------------|"
#comment "| OS List:                                                |"
#comment "| Linux,    RTT,    Debian,    Buildroot,    HarmonyOS    |"
#TODO
#add more here
#comment "xxx xxx"
#comment "|---------------------------------------------------------|"
#comment ""

choice
    prompt "OS type:"
    default OS_LINUX

config OS_LINUX
    bool "Linux"
    help
      Configuration for Linux OS.

config OS_RTT
    bool "RTT"
    help
      Configuration for RTT OS.

config OS_DEBIAN
    bool "Debian"
    depends on n
    help
      Configuration for Debian OS.

config OS_BUILDROOT
    bool "Buildroot"
    depends on n
    help
      Configuration for Buildroot OS.

config OS_HARMONYOS
    bool "HarmonyOS"
    depends on n
    help
      Configuration for HarmonyOS OS.

## Please add new OS here
## TODO

endchoice

# Include configuration if selected
if OS_LINUX
source "build/kconf/Kconfig.linux"
endif
if OS_RTT
source "build/kconf/Kconfig.rtt"
endif
#if OS_DEBIAN
#gsource "build/kconf/Kconfig.debian"
#endif
#if OS_BUILDROOT
#gsource "build/kconf/Kconfig.buildroot"
#endif
#if OS_HARMONYOS
#gsource "build/kconf/Kconfig.HarmonyOS"
#endif

endmenu

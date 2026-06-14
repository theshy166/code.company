SHELL := /bin/bash

export SDK_ROOT=$(shell pwd)

export BSP_DIR=$(SDK_ROOT)/bsp
export MEDIA_DIR=$(SDK_ROOT)/media
export MPP_DIR=$(SDK_ROOT)/media/mpp
export OUTPUT_ROOT_DIR=$(SDK_ROOT)/output
export PREBUILTS_DIR=$(SDK_ROOT)/prebuilts/${CONFIG_XOS_PROJECT_TYPE}
PUB_ROOTFS=bsp/rootfs

RTT_FASTBOOT?=N
ENV_FILE=${OUTPUT_ROOT_DIR}/.config
ifeq (,$(wildcard $(ENV_FILE)))
    $(error "Please use "./envsetup.sh" to select a device config")
endif
include $(ENV_FILE)

GCC_INFO := $(shell $(CROSS)gcc -v 1>&2 2> /dev/null; echo $$?)
ifneq ($(GCC_INFO),0)
    $(info Not found: $(CROSS)gcc)
	$(error "Please add the cross compiler into your system path!!")
endif

LIB_RUNTIME:=$(CROSS_RUNTIME_LIB)

FAKEROOT_INFO := $(shell fakeroot -- ls &> /dev/null; echo $$?)
ifneq ($(FAKEROOT_INFO),0)
    $(info Not found: faketroot)
	FAKEROOT_CMD :=@
else
	FAKEROOT_CMD :=@fakeroot --
endif

export OUTPUT_DIR=$(OUTPUT_ROOT_DIR)/$(TARGET_OUTPUT_DIR)

export IMAGE_CLASS?=boot
export post_bin_path?=$(OUTPUT_DIR)/
export FB?=0
export RTT_AP?=0

# get the isp sub folder
VERS_DIR := $(CROSS_VID)

MEM_LAYOUT=mem_layout.sh

# Do we want to locate output files in a separate directory?
ifeq ("$(origin O)", "command line")
  BSP_BUILD_DIR := $(strip $(O))/
endif

.PHONY: info

info:
	@echo "        Board: $(TARGET_CONFIG_NIMI)"
	@echo "       Kernel: linux-$(KERNEL_VER)"
	@echo " Uboot config: $(UBOOT_CONFIG)"
	@echo "Kernel config: $(KERNEL_CONFIG)"
	@echo "   Kernel DTS: $(KERNEL_DTS)"
	@echo "        CROSS: $(CROSS)"


.PHONY: all
all:  boot kernel driver lib sample bsp_test rootfs

.PHONY: clean
clean:  boot_clean kernel_clean driver_clean lib_clean sample_clean bsp_test_clean busybox_clean boardtools_clean  rootfs_clean


##########################################################################################
#task [0]	prepare
##########################################################################################
.PHONY: prepare prepare_output prepare_banner

prepare_banner:
	@echo -e "\e[0;32;1m == Building for $(TARGET_CONFIG_NIMI) ...\e[0;36;1m \e[0m"

prepare_output:
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(OUTPUT_DIR)/media/{ko,obj,lib,sample}
	@mkdir -p $(OUTPUT_DIR)/bsp/{kernel,boot,rootfs,test}



prepare: prepare_banner prepare_output


##########################################################################################
#task [1]	boot
##########################################################################################
.PHONY: boot ramboot boot_prepare boot_build boot_post boot_clean

boot_prepare: prepare
	@echo "---------task [boot] prepare "
	# cd $(MEDIA_DIR)/mpp/configs;./$(MEM_LAYOUT);cd -;
	@if [ -d "$(MPP_DIR)/prebuilt/" ]; then \
		cp $(MPP_DIR)/prebuilt/* $(OUTPUT_DIR)/ -rvf; \
	else \
		echo "no $(MPP_DIR)/prebuilt, ignore boot_prepare"; \
	fi

boot_build:
	@echo "---------task [boot] build"
	@if [ -d "$(BSP_DIR)/uboot/" ]; then \
		make -C $(BSP_DIR)/uboot/$(UBOOT_VER) ARCH=arm CROSS_COMPILE=$(CROSS) distclean || exit 1; \
		make -C $(BSP_DIR)/uboot/$(UBOOT_VER) ARCH=arm CROSS_COMPILE=$(CROSS) $(UBOOT_CONFIG) || exit 1; \
		make -C $(BSP_DIR)/uboot/$(UBOOT_VER) ARCH=arm CROSS_COMPILE=$(CROSS) -j 4 || exit 1; \
	else \
		echo "no $(BSP_DIR)/boot, ignore boot_build"; \
	fi

ramboot:
	@echo "---------task [ramboot] build"
	@if [ -d "$(BSP_DIR)/ramboot/" ]; then \
		make -C $(BSP_DIR)/ramboot ARCH=arm CROSS_COMPILE=$(CROSS) distclean; \
		make -C $(BSP_DIR)/ramboot ARCH=arm CROSS_COMPILE=$(CROSS) $(UBOOT_CONFIG); \
		make -C $(BSP_DIR)/ramboot ARCH=arm CROSS_COMPILE=$(CROSS) -j 4; \
	else \
		echo "no $(BSP_DIR)/ramboot, ignore built ramboot"; \
	fi


boot_post:
	@echo "---------task [boot] post"

	@mkdir -p $(OUTPUT_DIR)/bsp/pdl/
	@mkdir -p $(OUTPUT_DIR)/bsp/spl/
	@if [ -d "$(BSP_DIR)/ramboot/" ]; then \
	cp $(BSP_DIR)/ramboot/$(BSP_BUILD_DIR)spl/u-boot-spl-header.img $(OUTPUT_DIR)/bsp/spl/u-boot-spl-header.img; \
	cp $(BSP_DIR)/ramboot/$(BSP_BUILD_DIR)pdl/u-boot-pdl-header.img $(OUTPUT_DIR)/bsp/pdl/u-boot-pdl-header.img; \
	else \
		echo "no $(BSP_DIR)/ramboot, copy $(PREBUILTS_DIR)/bsp/*"; \
		cp $(PREBUILTS_DIR)/bsp/spl/u-boot-spl-header.img $(OUTPUT_DIR)/bsp/spl/u-boot-spl-header.img; \
		cp $(PREBUILTS_DIR)/bsp/pdl/u-boot-pdl-header.img $(OUTPUT_DIR)/bsp/pdl/u-boot-pdl-header.img; \
	fi
	@if [ -d "$(BSP_DIR)/uboot/$(UBOOT_VER)" ]; then \
	cp $(BSP_DIR)/uboot/$(UBOOT_VER)/$(BSP_BUILD_DIR)u-boot.bin $(OUTPUT_DIR)/bsp/boot/ ; \
	cp $(BSP_DIR)/uboot/$(UBOOT_VER)/$(BSP_BUILD_DIR)u-boot-dtb.bin $(OUTPUT_DIR)/bsp/boot/ ; \
	cp $(BSP_DIR)/uboot/$(UBOOT_VER)/$(BSP_BUILD_DIR)sdl/u-boot-sdl.bin $(OUTPUT_DIR)/bsp/boot/ ; \
	else \
		echo "no $(BSP_DIR)/uboot/$(UBOOT_VER), ignore boot_post"; \
	fi
	@echo "---------merge_bin_clean"
	@if [ -d "$(BSP_DIR)/ramboot/" ]; then \
	rm -f $(BSP_DIR)/ramboot/$(BSP_BUILD_DIR)*merge_bin*; \
	else \
		echo "no $(BSP_DIR)/ramboot, ignore boot_post"; \
	fi

boot: boot_prepare boot_build ramboot boot_post

boot_clean:
	@echo "---------task [boot] build"
	@if [ -d "$(BSP_DIR)/uboot/" ]; then \
	make -C $(BSP_DIR)/uboot/$(UBOOT_VER) ARCH=arm CROSS_COMPILE=$(CROSS) distclean || exit 1; \
	[ -d $(O) ] || (rm -rf $(BSP_DIR)/uboot/$(UBOOT_VER)/$(BSP_BUILD_DIR)); \
	else \
		echo "no $(BSP_DIR)/uboot, ignore boot_clean uboot"; \
	fi
	@if [ -d "$(BSP_DIR)/ramboot/" ]; then \
	make -C $(BSP_DIR)/ramboot ARCH=arm CROSS_COMPILE=$(CROSS) distclean || exit 1; \
	[ -d $(O) ] || (rm -rf $(BSP_DIR)/ramboot/$(BSP_BUILD_DIR)); \
	else \
		echo "no $(BSP_DIR)/ramboot, ignore boot_clean ramboot"; \
	fi


##########################################################################################
#task [2]	kernel
##########################################################################################
.PHONY: kernel kernel_prepare kernel_build kernel_post kernel_image_build kernel_image kernel_clean

kernel_prepare: prepare
	@echo "---------task [kernel] prepare "
	#cp $(MPP_DIR)/prebuilt/* $(OUTPUT_DIR)/ -rvf

kernel_build:
	@echo "---------task [kernel] build -j $$(($$(nproc)/1));"
	@if [ -d "$(BSP_DIR)/kernel/" ]; then \
	make -C $(BSP_DIR)/kernel/linux-$(KERNEL_VER) ARCH=arm CROSS_COMPILE=$(CROSS) distclean || exit 1; \
	make -C $(BSP_DIR)/kernel/linux-$(KERNEL_VER) ARCH=arm CROSS_COMPILE=$(CROSS) $(KERNEL_CONFIG) || exit 1; \
	make -C $(BSP_DIR)/kernel/linux-$(KERNEL_VER) ARCH=arm CROSS_COMPILE=$(CROSS) -j $$(($$(nproc)/1)) || exit 1; \
	else \
		echo "no $(BSP_DIR)/kernel, ignore kernel_build"; \
	fi

kernel_image_build:
	@echo "---------task [kernel image] build"
	@if [ -d "$(BSP_DIR)/kernel/" ]; then \
	make -C $(BSP_DIR)/kernel/linux-$(KERNEL_VER) ARCH=arm CROSS_COMPILE=$(CROSS) -j $$(($$(nproc)/1)) || exit 1; \
        else \
                echo "no $(BSP_DIR)/kernel, ignore kernel_image_build"; \
        fi	

kernel_post:
	@echo "---------task [kernel] post"
	@if [ -d "$(BSP_DIR)/kernel/" ]; then \
	cp $(BSP_DIR)/kernel/linux-$(KERNEL_VER)/$(BSP_BUILD_DIR)arch/arm/boot/zImage $(OUTPUT_DIR)/bsp/kernel/ || exit 1; \
	cp $(BSP_DIR)/kernel/linux-$(KERNEL_VER)/$(BSP_BUILD_DIR)arch/arm/boot/Image $(OUTPUT_DIR)/bsp/kernel/ || exit 1; \
	else \
		echo "no $(BSP_DIR)/kernel, ignore kernel_post"; \
	fi
	make -C $(PREBUILTS_DIR)/media/tools/post_build/ -f $(PREBUILTS_DIR)/media/tools/post_build/Makefile.mkimage || exit 1
	@if [ -d "$(BSP_DIR)/kernel/" ]; then \
	cp $(BSP_DIR)/kernel/linux-$(KERNEL_VER)/$(BSP_BUILD_DIR)arch/arm/boot/dts/$(KERNEL_DTS:.dts=.dtb) $(OUTPUT_DIR)/bsp/kernel/$(KERNEL_DTS:.dts=.dtb) || exit 1; \
	cat $(OUTPUT_DIR)/bsp/kernel/zImage $(OUTPUT_DIR)/bsp/kernel/$(KERNEL_DTS:.dts=.dtb) > $(OUTPUT_DIR)/bsp/kernel/zImage-dtb || exit 1; \
	cat $(OUTPUT_DIR)/bsp/kernel/Image $(OUTPUT_DIR)/bsp/kernel/$(KERNEL_DTS:.dts=.dtb) > $(OUTPUT_DIR)/bsp/kernel/Image-dtb || exit 1; \
	else \
		echo "no $(BSP_DIR)/kernel, ignore kernel_post"; \
	fi

kernel: kernel_prepare kernel_build kernel_post

kernel_image: kernel_image_build kernel_post


kernel_clean:
	@echo "---------task [kernel] clean"
	@if [ -d "$(BSP_DIR)/kernel/" ]; then \
	make -C $(BSP_DIR)/kernel/linux-$(KERNEL_VER) ARCH=arm CROSS_COMPILE=$(CROSS) distclean; \
	[ -d $(O) ] || (rm -rf $(BSP_DIR)/kernel/linux-$(KERNEL_VER)/$(BSP_BUILD_DIR)); \
	else \
		echo "no $(BSP_DIR)/kernel, ignore kernel_clean"; \
	fi


##########################################################################################
#task [3]	prepare rootfs
##########################################################################################
rootfs_prepare: prepare
	@echo "---------task [3] prepare rootfs "
	if [ -f "$(BSP_DIR)/rootfs/rootfs.tar.gz" ]; then \
            echo "rootfs.tar.gz found, preparing rootfs..."; \
            rm $(OUTPUT_DIR)/$(PUB_ROOTFS)/* -rf; \
            tar xzf $(BSP_DIR)/rootfs/rootfs.tar.gz -C $(OUTPUT_DIR)/bsp/; \
            tar xvf $(shell dirname $(shell which $(CROSS)gcc))/../$(LIB_RUNTIME).tar.bz2 -C $(OUTPUT_DIR)/bsp/rootfs; \
            pushd $(OUTPUT_DIR)/$(PUB_ROOTFS); \
            chmod -R 0755 $(LIB_RUNTIME); \
            mv $(LIB_RUNTIME)/* lib/; \
            rm -rf $(LIB_RUNTIME); \
            popd; \
        else \
            echo "$(BSP_DIR)/rootfs/rootfs.tar.gz not found, skipping rootfs preparation."; \
        fi

##########################################################################################
#task [4]	build busybox
##########################################################################################
ifeq ($(shell ls $(BSP_DIR)/busybox/$(BUSYBOX_VER)/_install 2>/dev/null), )
busybox: prepare
	@echo "---------task [4] build busybox"
	@if [ -d "$(BSP_DIR)/busybox/" ]; then \
		if [ ! -d $(BSP_DIR)/busybox/$(BUSYBOX_VER) ]; then \
			pushd $(BSP_DIR)/busybox/ && tar xvf $(BUSYBOX_VER).tar.bz2 && popd; \
		fi; \
		rm -rf $(BSP_DIR)/busybox/$(BUSYBOX_VER)/_install; \
		if [ -d $(BSP_DIR)/busybox/$(BUSYBOX_VER) ]; then \
			cd $(BSP_DIR)/busybox/$(BUSYBOX_VER); \
			ls ../patches/$(BUSYBOX_VER)/* 2>/dev/null | xargs -i patch --forward -p0 -i {}; \
		fi; \
		make CROSS_COMPILE=$(CROSS) -C $(BSP_DIR)/busybox/$(BUSYBOX_VER) distclean; \
		cp $(BSP_DIR)/busybox/$(BUSYBOX_CFG) $(BSP_DIR)/busybox/$(BUSYBOX_VER)/.config; \
		make CROSS_COMPILE=$(CROSS) -C $(BSP_DIR)/busybox/$(BUSYBOX_VER) -j 20; \
		make CROSS_COMPILE=$(CROSS) -C $(BSP_DIR)/busybox/$(BUSYBOX_VER) install; \
		cp -avf $(BSP_DIR)/busybox/$(BUSYBOX_VER)/_install/* $(OUTPUT_DIR)/$(PUB_ROOTFS); \
		pushd $(OUTPUT_DIR)/$(PUB_ROOTFS)/ && ln -s bin/busybox init && popd; \
		mkdir -p $(OUTPUT_DIR)/bsp/rootfs/usr/share/udhcpc; \
		cp -af $(BSP_DIR)/busybox/$(BUSYBOX_VER)/examples/udhcp/simple.script $(OUTPUT_DIR)/$(PUB_ROOTFS)/usr/share/udhcpc/default.script; \
	else \
		echo "$(BSP_DIR)/busybox/ not found, skipping busybox preparation."; \
	fi
else
busybox: prepare
	@echo "---------task [4] busybox is ready "
	@if [ -d "$(BSP_DIR)/busybox/" ]; then \
	cp -avf $(BSP_DIR)/busybox/$(BUSYBOX_VER)/_install/* $(OUTPUT_DIR)/$(PUB_ROOTFS); \
	pushd $(OUTPUT_DIR)/$(PUB_ROOTFS)/ && ln -s bin/busybox init && popd; \
	mkdir -p $(OUTPUT_DIR)/bsp/rootfs/usr/share/udhcpc; \
	cp -af $(BSP_DIR)/busybox/$(BUSYBOX_VER)/examples/udhcp/simple.script $(OUTPUT_DIR)/$(PUB_ROOTFS)/usr/share/udhcpc/default.script; \
	else \
		echo "$(BSP_DIR)/busybox/ not found, skipping busybox preparation."; \
	fi
endif


busybox_clean:
	echo fake clean, to delete $(BSP_DIR)/busybox/$(BUSYBOX_VER) to rebuild busybox
	#make -C $(BSP_DIR)/busybox/$(BUSYBOX_VER) distclean

##########################################################################################
#task [5]	build pc tools
##########################################################################################
pctools: prepare
	@echo "---------task [5] build tools which run on pc"

pctools_clean:


##########################################################################################
#task [6]	build board tools
##########################################################################################
boardtools: rootfs_prepare boardtools_clean
	@echo "---------task [6] build tools which run on board "
	make -C $(PREBUILTS_DIR)/media/tools/board/udev-164 OSDRV_CROSS=$(CROSS) || exit 1;
	cp -af $(PREBUILTS_DIR)/media/tools/board/udev-164/rootfs/. $(OUTPUT_DIR)/$(PUB_ROOTFS)/;
	make -C $(PREBUILTS_DIR)/media/tools/board/udev-164 OSDRV_CROSS=$(CROSS) clean || exit 1;

boardtools_clean:


##########################################################################################
#task [7]	build rootfs
##########################################################################################
rootfs: rootfs_prepare busybox boardtools

yaffs2:
	@echo "---------task [7] build yaffs image!"
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mkyaffs2.sh  $(OUTPUT_DIR)/$(PUB_ROOTFS) rootfs.yaffs2.img
	@echo "---------finish compile work"
squashfs:
	@echo "---------task [7] build squashfs image!"
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mksquashfs.sh  $(OUTPUT_DIR)/$(PUB_ROOTFS) rootfs.squash.img
	@echo "---------finish compile work"

ramdisk:
	@echo "---------task [7] build rootfs"
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mkinitramfs.sh  $(OUTPUT_DIR)/$(PUB_ROOTFS) $(BSP_DIR)/kernel/linux-$(KERNEL_VER)

	@echo "  generated $(BSP_DIR)/rootfs/initramfs.cpio.xz"
	@echo "---------finish compile work"

jffs2:
	@echo "---------task [7] build rootfs"
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mkjffs2.sh $(OUTPUT_DIR)/$(PUB_ROOTFS) 0x10000 rootfs.jffs2.64k.img
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mkjffs2.sh $(OUTPUT_DIR)/$(PUB_ROOTFS) 0x20000 rootfs.jffs2.128k.img
	$(FAKEROOT_CMD) $(PREBUILTS_DIR)/media/tools/pc/fs_tools/mkjffs2.sh $(OUTPUT_DIR)/$(PUB_ROOTFS) 0x40000 rootfs.jffs2.256k.img

rootfs_clean:
	rm $(OUTPUT_DIR)/bsp/rootfs/ -rf

##########################################################################################
#task [8]	drivers
##########################################################################################
.PHONY: driver driver_prepare driver_build driver_post driver_clean

driver_prepare: prepare
	@echo "---------task [driver] prepare"
#	cp $(MPP_DIR)/prebuilt/ko/loadko.sh $(OUTPUT_DIR)/media/ko/
#	cp $(MPP_DIR)/prebuilt/ko/loadko_maxlod_24M.sh $(OUTPUT_DIR)/media/ko/

driver_build:
	set -e;if [ "$(RTT_FASTBOOT)" == "Y" ] ; then \
		make -C $(MEDIA_DIR)/driver xbus=y fboot=y; \
	else \
		make -C $(MEDIA_DIR)/driver; \
		make -C $(MEDIA_DIR)/driver clean; \
#		make -C $(MEDIA_DIR)/driver xbus=y; \
	fi


driver_post:
	@echo "---------task [driver] post"
	cp -p $(MPP_DIR)/ko/$(LOADKO_SCRIPT) $(OUTPUT_DIR)/media/ko/loadko.sh
	cp $(MPP_DIR)/ko/$(VERS_DIR)/nnp.ko $(OUTPUT_DIR)/media/ko/
	cp $(MEDIA_DIR)/isp/isp_main/$(VERS_DIR)/ko/isp.ko $(OUTPUT_DIR)/media/ko/
	cp $(MEDIA_DIR)/isp/isp460/ko/isp460.ko $(OUTPUT_DIR)/media/ko/

driver: driver_prepare driver_build
#driver: driver_prepare driver_build driver_post
#driver:
	$(info "disabled by siyuan.song")
	$(CC) -v
driver_clean:
	@echo "---------task [driver] clean"
	make -C $(MEDIA_DIR)/driver clean


##########################################################################################
#task [9]	libs
##########################################################################################
.PHONY: lib lib_prepare lib_build lib_post lib_clean

lib_prepare: prepare
	@echo "---------task [lib] prepare "

lib_build:
	@echo "---------task [lib] build"
	make -C $(MEDIA_DIR)/mpp/mpi


lib_post:
	@echo "---------task [lib] post"
	mkdir -p $(MEDIA_DIR)/lib/isp460
	rsync -av --exclude v510 --exclude v512 --exclude v610 $(MEDIA_DIR)/lib/* $(OUTPUT_DIR)/media/lib/
#	- cp -raf $(MEDIA_DIR)/isp/isp_main/$(VERS_DIR)/lib/lib*.a $(MEDIA_DIR)/isp/isp_main/lib
#	- cp -raf $(MEDIA_DIR)/isp/isp_main/$(VERS_DIR)/lib/lib*.a $(OUTPUT_DIR)/media/lib/
#	- cp -raf $(MEDIA_DIR)/isp/isp460/lib/lib*.a $(OUTPUT_DIR)/media/lib/isp460
#	- cp -raf  $(MEDIA_DIR)/lib/$(VERS_DIR)/* $(OUTPUT_DIR)/media/lib/
lib: lib_prepare lib_build lib_post
#lib:
	$(info "disabled by siyuan.song")
	$(CC) -v


lib_clean:
	@echo "---------task [lib] cilean"
	make -C $(MEDIA_DIR)/mpp/mpi clean
##########################################################################################
#task [10]	sample
##########################################################################################
.PHONY: sample sample_prepare sample_build sample_post sample_clean

sample_prepare: prepare
	@echo "---------task [sample] prepare "

sample_build:
	@echo "---------task [sample] build"
	 make -C $(MEDIA_DIR)/sample


sample_post:
	@echo "---------task [sample] post"

sample: sample_prepare sample_build sample_post

sample_clean:
	@echo "---------task [sample] clean"
	make -C $(MEDIA_DIR)/sample clean

##########################################################################################
#task [11]	bsp test
##########################################################################################
.PHONY: bsp_test bsp_test_clean

bsp_test:
	@echo "---------task [sample] build"
	 make -C $(BSP_DIR)/test
	 cp $(MEDIA_DIR)/tools/board/linux/molv2/* $(OUTPUT_DIR)/bsp/test/

bsp_test_clean:
	@echo "---------task [sample] clean"
	make -C $(BSP_DIR)/test clean

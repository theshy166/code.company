cmd_arch/arm/dts/molchip-v2p-ca7s-lt00.dtb := mkdir -p arch/arm/dts/ ; cat ../arch/arm/dts/molchip-v2p-ca7s-lt00.dts  | arm-molv2-linux-uclibcgnueabi-gcc -E -Wp,-MD,arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.d.pre.tmp -nostdinc -I../arch/arm/dts -I../arch/arm/dts/include -Iinclude -I../include -I../arch/arm/include -include ../include/linux/kconfig.h -D__ASSEMBLY__ -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.dts.tmp - ; dtc -O dtb -o arch/arm/dts/molchip-v2p-ca7s-lt00.dtb -b 0 -i ../arch/arm/dts/ -Wno-unit_address_vs_reg -Wno-simple_bus_reg -Wno-unit_address_format -Wno-pci_bridge -Wno-pci_device_bus_num -Wno-pci_device_reg -d arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.d.dtc.tmp arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.dts.tmp ; cat arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.d.pre.tmp arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.d.dtc.tmp > arch/arm/dts/.molchip-v2p-ca7s-lt00.dtb.d

source_arch/arm/dts/molchip-v2p-ca7s-lt00.dtb := ../arch/arm/dts/include/dt-bindings/interrupt-controller/arm-gic.h

deps_arch/arm/dts/molchip-v2p-ca7s-lt00.dtb := \
  ../arch/arm/dts/include/dt-bindings/interrupt-controller/irq.h \
  ../arch/arm/dts/include/dt-bindings/net/mc_ephy.h \
  ../arch/arm/dts/include/dt-bindings/gpio/gpio.h \

arch/arm/dts/molchip-v2p-ca7s-lt00.dtb: $(deps_arch/arm/dts/molchip-v2p-ca7s-lt00.dtb)

$(deps_arch/arm/dts/molchip-v2p-ca7s-lt00.dtb):

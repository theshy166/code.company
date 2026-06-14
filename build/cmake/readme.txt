【编译】
cd xos
source build/envsetup.sh
lunch   // 目前可以选择 2: Rtt SDK
然后
make menuconfig   // 可选操作
make xos -j8

【输出路径】
目前暂时把所有cmake的生产路径放在 xos/output (如需修改，请修改build/envsetup.sh的lunch()函数)
会在output下生成xos.elf

【kconfig】
在顶层的build/Kconfig中，可使用
source "xxx/Kconfig"
添加在某个路径下的Kconfig

【CMakeLists.txt中使用Kconfig】
可以使用例如：
if (DEFINED CONFIG_SDK_IS_MOLCHIP_FY00_RTT)

[C代码中使用Kconfig]
需要
#include "output/kconfig/include/generated/autoconf.h"
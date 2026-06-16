1. pro.sh用来处理spl镜像,使用示例如下:
Romcode校验SPL数据
./pro.sh  u-boot-spl.bin  s.bin  u-boot-spl.bin  30000800
 u-boot-spl.bin为库上编译出来的spl镜像,s.bin为中间临时镜像30000800(16进制)是将u-boot-spl.bin数据从flash颗粒中读到的地址
 
 
2. pro.sh用来处理pre_rolling镜像,使用示例如下:
 ./pro.sh  xc01_uart.bin  pro.bin xc01_uart.bin  30010000
 xc01_uart.bin为调试pro_rolling使用的镜像(可更改),pro.bin为中间临时镜像,30010000(16进制)是将xc01_uart.bin数据从flash颗粒中读到的地址
 
3.rtt.sh用来处理rtt0和rtt1镜像,使用示例如下:
 ./rtt.sh   rtthread.bin hh.bin rtthread_cp.bin 50000000 50300000
 
 其中rtthread.bin 和 rtthread_cp.bin分别为ARCH0和ARCH1使用的bin,hh.bin为生成的临时文件不用关注,50000000和50300000(16进制)分别为ARCH0和ARCH1执行的地址
 
4.kernel.sh用来处理kernel和DTB镜像,使用示例如下:
./kernel.sh  Image  kk.bin molchip-kernel-xc01.dtb  40008000 41000000
其中Image为内核编译出来的镜像,kk.bin为中间生成临时文件molchip-kernel-xc01.dtb为内核编译生成的DTB镜像,40008000和41000000(16进制)分别为内核image和dtb加载的地址
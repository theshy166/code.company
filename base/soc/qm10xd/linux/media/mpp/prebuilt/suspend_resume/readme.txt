BSP+DSP 休眠唤醒测试：
1. kernel启动到控制台后，挂载nfs

2.  将suspend_resume文件夹复制到根目录下，将sfc_test 复制到bin目录下
cp -rf /nfs/lt00/suspend_resume/ / 
cp -rf /nfs/lt00/suspend_resume/scripts/sfc_test /bin 

3.  加载多媒体ko
cd /nfs/lt00/media/ko \r\n sh loadko.sh -i 

4. 执行脚本
cd /suspend_resume/scripts 
 sh BSP-test-all.sh& 
 sh check_dummy_reg.sh&
 sh mem.sh & 

cd /nfs/lt00/media/sample 
./vdppo_test -t h264 -s stream/h264_1920_1080.h264 -r 1920x1080 -b -f 30 -l 1 -d 999999999 -m vdppo_bsp & 
（如果需要等bsp done才睡眠，用vddpo_bsp， 如果在bsp test过程中允许睡眠，用vddpo。 此处用vddpo_bsp）

BSP only 休眠唤醒测试：
1. kernel启动到控制台后，挂载nfs

2.  将suspend_resume文件夹复制到根目录下，将sfc_test 复制到bin目录下
cp -rf /nfs/lt00/suspend_resume/ / 
cp -rf /nfs/lt00/suspend_resume/scripts/sfc_test /bin 

3. 执行脚本：
cd /suspend_resume/scripts 
 ./BSP-test-all.sh& 
 ./BSP-test-sleep_only.sh& 



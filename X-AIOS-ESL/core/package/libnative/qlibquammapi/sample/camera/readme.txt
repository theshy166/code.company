camera sample使用步骤
一、如何编译新的sensor(test_mipi)库
1、参考SDK/base/soc/qm10xh/linux/media/tools/board/sensor-build/isp/libs/sensor目录里已实现sensor，完成代码并编译成libtest_mipi.so库
2、把libtest_mipi.so库拷贝到板子目录/vendor/qua/lib

二、如何增加新的sensor(test_mipi)配置文件
1、新建sensor配置文件QuaCameraConfig
2、根据硬件接线和sensor信息，填入如下信息
dev_num:1
dev_tag:begin
dev_id:0
dev_iic:1
sns_num:1
sns_tag:begin
sns_name:test_mipi
sns_width:3840
sns_height:2160
sns_fps:30
sns_wdr:0
sns_tag:end
dev_tag:end
3、把QuaCameraConfig配置文件拷贝到板子目录/data/sensor_param

三、编译与运行sample
1、执行quammapi目录buildmmapi.sh脚本，编译出mpp_camera_pipeline_sample
2、mpp_camera_pipeline_sample拷贝到板子上执行

# sensor库编译环境

## 编译方法
1. 添加编译器路径到 PATH 环境变量:
    - 确认需要使用的编译器

    - 执行:

            export PATH=编译器所在路径/bin:$PATH

2. 运行 ./setup_env.sh 生成 personal.make

3. 运行 build.sh 进行编译:

        ./build.sh

4. 编译好的sensor库放在 _output 文件夹中



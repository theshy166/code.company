不要参考:安全启动方案介绍.pdf
请参考下列:
SDK编译成果物相关名字如下(在\output\lt00\bsp目录下对应名称文件夹):
u-boot-pdl-header.img
u-boot-spl-header.img
u-boot.bin
将上述三个镜像需要处理的copy到本目录下
注意:下属第2点pdl.sh、spl.sh以及boot.sh分别对上述三个成果物处理的脚本
下属第三点开始以使用./pdl.sh对u-boot-pdl-header.img处理生成的pdl.bin进行加签加密处理流程

1  To generate a key-pair:
   Execute the command:
   openssl genrsa -out rsa_priv.pem 2048

   Upon the successful execution, openssl generates the key file named
   rsa_priv.pem file.
2 process bin file   
	执行./pdl.sh 会生成pdl.bin
	执行./spl.sh 会生成spl.bin
	执行./boot.sh 会生成boot.bin
   
3  Use rsa_sign.sh signing the image file
	以处理pdl.bin为示例:
   ./rsa_sign.sh pdl.bin rsa_priv.pem 0x400 pss
    0x400 is the image header length
    
4  Write the aes key and iv to the config file
	aes.cfg里面存了KEY和IV
5  Use aes_encode.sh encoding the image file
   ./aes_encode.sh pdl.bin  aes.cfg 0x1f0
   0x1f0是指的从文件偏移0x1f0的位置开始加密
 

例子:

1 进行RSA加签（rsa_priv.perm已生成）
./rsa_sign.sh pdl.bin rsa_priv.pem 0x400 pss
会生成bsp/uboot/uboot/scripts/secure/secure_doc/spl.bin.pubkey.tmp)= 5c6cc783c765fd371afc39b623b56e68b8c869a3bc6be1189e69df181db21c46
即公钥的HASH

2 配置aes.cfg文件中的KEY和IV
KEY=13246BE7E1008B951110009325241312
IV=262738393a3b4c4d4e4f000000000000

3 进行AES CBC对称加密
./aes_encode.sh pdl.bin  aes.cfg 0x1f0

4 往EFUSE中写公钥的哈希值,block4开始
uboot下写示例(注意大小端):
efuse_write 0 4 0x83c76c5c
efuse_write 0 5 0x37fd65c7
efuse_write 0 6 0xb639fc1a
efuse_write 0 7 0x686eb523
efuse_write 0 8 0xa369c8b8
efuse_write 0 9 0x18e16bbc
efuse_write 0 10 0x18df699e
efuse_write 0 11 0x461cb21d


5 往EFUSE中写AES KEY,block0开始
efuse_write 0 0 0x13246BE7
efuse_write 0 1 0xE1008B95
efuse_write 0 2 0x11100093
efuse_write 0 3 0x25241312

6 使能安全启动
efuse_write 0 15 0x030fff07
efuse_write 0 12 0x00000005



注意:
1. 烧录EFUSE顺序,烧录公钥HASH以及AES KEY后再使能安全启动
2. 目前romcode 会校验SPL以及PDL,PDL不校验SDL,所以不用对SDL进行处理
3. 使能安全启动后下载加密加签处理后的镜像



















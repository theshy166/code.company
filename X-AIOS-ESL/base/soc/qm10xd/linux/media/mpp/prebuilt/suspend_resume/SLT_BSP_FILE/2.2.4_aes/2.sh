#!/bin/sh
set -x

SDC_MNT=.

src_file1="src_file1.txt"
src_file2="src_file2.txt"

[[ ! -f "$SDC_MNT/$src_file1" ]] && dd if=/dev/urandom of=${SDC_MNT}/${src_file1} bs=1024 count=16
#[[ ! -f "$SDC_MNT/$src_file2" ]] && dd if=/dev/urandom of=${SDC_MNT}/${src_file2} bs=1024 count=100

if [[ ! -f "/${SDC_MNT}/aes_bigfile_test" ]]; then
    cp aes_bigfile_test $SDC_MNT
fi

cd $SDC_MNT

./aes_bigfile_test -m aes_ecb_128 -s $src_file1 -e enc_file1.txt -d dst_file1.txt 
#./aes_bigfile_test -m aes_ctr_128 -s $src_file2 -e enc_file2.txt -d dst_file2.txt &
set +x
echo "test over"
sync
#sync

aes_pass=`diff src_file1.txt dst_file1.txt`
if [ -z "$aes_pass" ];then
	#rm src_file1.txt 
	#rm dst_file1.txt
	echo "aes_test pass"
    echo "aes_test pass" >> /slt_log/slt_log.log
	return 0
else
	echo "aes_test fail"
	echo -e "\033[31merror:AES Test failed,please check\033[0m" >> /slt_log/slt_log.log
	return 1
fi

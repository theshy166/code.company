#!/bin/bash

usage()
{
    echo "sudo ./tools/ubuntu/strings/trans_strings_res.sh $__PRODUCT_NAME"
    echo "such as:"
	echo "    sudo./tools/ubuntu/strings/trans_strings_res.sh kidcamera"
	echo "    sudo./tools/ubuntu/strings/trans_strings_res.sh kidcamera_nor"
    exit 1
}

# 参数检查：确保传入2个参数
if [ $# -ne 1 ]; then
    echo "ERROR: Incorrect number of arguments, 1 product arguments are required."
    usage
fi

__PRODUCT_NAME=./product/$1
echo ""
echo "step1: trans *strings.xlsx $__PRODUCT_NAME"
./tools/ubuntu/strings/extract_strings_from_xlsx $__PRODUCT_NAME/src/strings/xls/strings.xlsx $__PRODUCT_NAME/src/strings/src

echo ""
echo "trans&copy done"
echo ""

#clean temp dir
rm -rf tools/ubuntu/strings/strings_src
echo "clean temp dir ok"
echo ""

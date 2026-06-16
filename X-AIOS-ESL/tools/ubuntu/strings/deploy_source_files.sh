#!/bin/sh
# copy ‘the language and strings source files’ to relational directory.
#
ori_src_dir=$1
echo "    SOURCE ori_src_dir=$1"

# ../../../core/res/res/strings_xls/

#target evb dir
dst_top_dir=core/res/strings
echo "    TARGET evb_tgt_dir=${dst_top_dir}"

#src dir
if [ -d ${ori_src_dir} ]; then
    echo "    source dir1:${ori_src_dir} exist"
    #tgt dir
    if [ -d ${dst_top_dir} ]; then
        echo "    target dir2:${dst_top_dir} exist! "
        echo "    start copying files..."

        tgt_hdr_dir=${dst_top_dir}/../inc
        if test -d $tgt_hdr_dir; then
            # copy .h
            cp -rf ./$1/xos_*.h $tgt_hdr_dir
            echo "    target dir3:${tgt_hdr_dir} copied."

            # copy .c
            tgt_source_dir=${dst_top_dir}/src/
            if test -d $tgt_source_dir; then
                cp -rf ./$1/raw_*.c    $tgt_source_dir
                cp -rf ./$1/zipped_*.c $tgt_source_dir
                echo "    target dir4:${tgt_source_dir} copied."
                echo "    copy files to evb, done"
            else
                echo "    target dir4:${tgt_source_dir} does not exist! quit4."
            fi
        else
            echo "    target dir3:${tgt_hdr_dir} does not exist! quit3."
        fi
    else
        echo "    target dir2:${dst_top_dir} does not exist! quit2."
    fi
else
    echo "    source dir1:${ori_src_dir} does not exist! quit1."
fi

echo ""

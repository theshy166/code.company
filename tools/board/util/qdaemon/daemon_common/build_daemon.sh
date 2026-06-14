#!/bin/sh
if [ -z $1 ];then
  echo "usage: $0 <platform as qm10xd/qm10xv/qm10xh/mc3310>"
  exit 1
fi
function apath()
{
  REALPATH=$(realpath $1)
  echo $REALPATH
  PATH=$REALPATH:$PATH
}
pushd ../../../../../build/script/common/
. ./xosenv.sh $1
popd
if [ $1 != "mc331x" ];then
libpath=$1
else
libpath=fy00
fi
CFLAGS="${CFLAGS:-}"
echo CFLAGS-01: $CFLAGS
LOCAL_CFLAGS+=" -std=c99 -Wall -lsysutils -L./../../../../../core/package/libnative/qlibquadrv/${libpath}_linux -Wl,-rpath=/system/qua/lib "
CFLAGS+=$LOCAL_CFLAGS
export CFLAGS
#$CC -v -std=c99 daemon.c -o daemon
rm ./daemon > /dev/null
if [  $1 == "mc331x" ];then
$CC -v $CFLAGS -DMC331X daemon.c -o daemon
$STRIP daemon
chmod +x daemon
cp -v daemon ../../../../../product/xmphoto/rootfs_overlay/bin/daemon
elif  [  $1 == "qm10xd" ];then
$CC -v $CFLAGS -DBIND_WDT2QXOSUI -DXKPHOTO daemon.c -o daemon
$STRIP daemon
chmod +x daemon
cp -v daemon ../../../../../product/xkphoto/rootfs_overlay/bin/daemon
$CC -v $CFLAGS -DBIND_WDT2QXOSUI -DESL  daemon.c -o esl_daemon
$STRIP esl_daemon
chmod +x esl_daemon
cp -v esl_daemon ../../../../../product/esl/rootfs_overlay/bin/daemon
cp -v esl_daemon ../../../../../product/esl2/rootfs_overlay/bin/daemon
elif  [  $1 == "qm10xv" ];then
$CC -v $CFLAGS -DQM10XV daemon.c -o daemon
$STRIP daemon
chmod +x daemon
cp -v daemon ../../../../../product/kidcamera/rootfs_overlay/bin/daemon
cp -v daemon ../../../../../product/ipc/rootfs_overlay/bin/daemon
elif  [  $1 == "qm10xh" ];then
$CC -v $CFLAGS -DQM10XH daemon.c -o daemon
$STRIP daemon
chmod +x daemon
cp -v daemon ../../../../../product/smartdoor/rootfs_overlay/bin/daemon
cp -v daemon ../../../../../product/aicamera/rootfs_overlay/bin/daemon
fi

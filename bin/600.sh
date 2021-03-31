#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1
d500="$INDIR/500-Mouthguard-Export"
d600="$INDIR/600-Mouthguard-PrintReady"

if [ ! -d $d500  ]; then
   echo "$d500 does not exist. Try again."
   exit
fi

TOPDIR="${INDIR%/*}"
CURR="$(basename -- $INDIR)"

echo $TOPDIR
echo $CURR

rm -rf $d600
mkdir $d600

meshlabserver -l $d600/meshlab.log -i $d500/mesh.stl -o $d600/shell.stl -m vn fn -s /app/etc/600.mlx


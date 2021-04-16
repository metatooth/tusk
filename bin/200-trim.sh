#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR/100" ]; then
   echo "$INDIR/100 does not exist. Try again."
   exit
fi

TOPDIR="${INDIR%/*}"
CURR="$(basename -- $INDIR)"

echo $TOPDIR
echo $CURR

rm -rf $INDIR/200
mkdir $INDIR/200

for f in $INDIR/100/*.ply; do
    next="$(basename -- $f)"
    ~/metaspace/meshlab/distrib/meshlabserver -l $INDIR/200/meshlab.log -i $f -o $INDIR/200/$next -m vn fn -s etc/200-trim.mlx    
done

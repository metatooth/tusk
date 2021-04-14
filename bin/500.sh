#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR/400" ]; then
   echo "$INDIR/400 does not exist. Try again."
   exit2
fi

TOPDIR="${INDIR%/*}"
CURR="$(basename -- $INDIR)"

echo $TOPDIR
echo $CURR

rm -rf $INDIR/500
mkdir $INDIR/500

rm -rf $INDIR/output

for f in $INDIR/400/*.stl; do
    next="$(basename ${f})"
    ~/metaspace/meshlab/distrib/meshlabserver -l $INDIR/500/meshlab.log -i $f -o $INDIR/500/shell-$next -m vn fn -s etc/500-a.mlx
    _build/bin/tusk/tusk extrude 20 $f $INDIR/500/model-$next
done

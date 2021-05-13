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

MSHLB=~/metaspace/meshlab/distrib/meshlabserver
LOG=$INDIR/200/meshlab.log 

$MSHLB -l $LOG -i $INDIR/100/100.ply -o $INDIR/200/200-trimmed.ply -m vn fn -s etc/select-faces-by-color.mlx
$MSHLB -l $LOG -i $INDIR/200/200-trimmed.ply -o $INDIR/200/210-merged.ply -m vn fn -s etc/merge-close-vertices.mlx    
$MSHLB -l $LOG -i $INDIR/200/210-merged.ply -o $INDIR/200/220-repaired.ply -m vn fn -s etc/repair-non-manifold-edges-by-removing-faces.mlx


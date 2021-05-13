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

MSHLB=~/metaspace/meshlab/distrib/meshlabserver

$MSHLB -l $INDIR/500/meshlab.log -i $INDIR/400/400.ply -o $INDIR/500/500-shell.ply -m vn fn -s etc/500-shell.mlx
_build/bin/tusk/tusk extrude 10 $INDIR/400/400.ply $INDIR/500/500-extruded.ply
$MSHLB -l $INDIR/500/meshlab.log -i $INDIR/500/500-extruded.ply -o $INDIR/500/500-model.ply -m vn fn -s etc/500-clean-and-repair.mlx

#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR/200" ]; then
   echo "$INDIR/200 does not exist. Try again."
   exit
fi

TOPDIR="${INDIR%/*}"
CURR="$(basename -- $INDIR)"

echo $TOPDIR
echo $CURR

rm -rf $INDIR/400
mkdir $INDIR/400

for f in $INDIR/200/*.ply; do
    next="$(basename ${f%.ply}.stl)"
    ~/metaspace/meshlab/src/distrib/meshlabserver -i $f -o $INDIR/400/$next
done

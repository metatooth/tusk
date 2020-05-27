#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR/300" ]; then
   echo "$INDIR/300 does not exist. Try again."
   exit
fi

TOPDIR="${INDIR%/*}"
CURR="$(basename -- $INDIR)"

echo $TOPDIR
echo $CURR

rm -rf $INDIR/400
mkdir $INDIR/400

for f in $INDIR/300/*.stl; do
    next="$(basename ${f})"
    ~/metaspace/meshlab/distrib/meshlabserver -l $INDIR/400/logfile.txt -i $f -o $INDIR/400/$next -m vn fn -s etc/400.mlx
done

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

for f in $INDIR/300/*.ply; do
    next="$(basename ${f})"
    _build/bin/tusk/tusk normalize $f $INDIR/400/$next
done

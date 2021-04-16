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

rm -rf $INDIR/300
mkdir $INDIR/300

for f in $INDIR/200/*.ply; do
  next="$(basename ${f})"
  _build/bin/tusk/tusk convention $f $INDIR/300/$next
done

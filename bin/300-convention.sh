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

_build/bin/tusk/tusk convention $INDIR/200/220-repaired.ply $INDIR/300/300.ply



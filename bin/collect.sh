#!/usr/bin/bash

if [ "$#" -ne 2 ]; then
    echo "usage: collect.sh <indir> <outdir>"
    echo
    echo "  Collects models from <indir> for manufacturing."
    echo "  Creating single data-stamped directory under <outdir>."
    echo
    echo "  for example, automate.sh ~/metaspace/PROD ~/metaspace/JOBS"
    exit
fi

INDIR=$1
OUTDIR=$2

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

if [ ! -d "$OUTDIR" ]; then
   echo "$OUTDIR does not exist. Try again."
   exit
fi


year=$(date --utc +%Y)
month=$(date --utc +%m)
day=$(date --utc +%d)

rm -rf $OUTDIR/$year-$month-$day
mkdir $OUTDIR/$year-$month-$day

echo $OUTDIR/$year-$month-$day/
echo find $INDIR -path $INDIR/_archive -prune -false -o -name model* -exec cp '{}' $OUTDIR/$year-$month-$day/ \;
find $INDIR -path $INDIR/_archive -prune -false -o -name 'model-*.stl' -exec cp '{}' $OUTDIR/$year-$month-$day/ \;

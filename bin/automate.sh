#!/usr/bin/bash

if [ "$#" -ne 2 ]; then
    echo "usage: automate.sh <indir> <outdir>"
    echo
    echo "  Executes process steps 200, 300, 400, and 500 for subdirectories"
    echo "  of <indir>."
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

for d in $INDIR/*; do
    bname=$(basename $d)
    if [[ "$bname" == "_archive" ]]; then
     echo "$bname is _archive"
     echo "Will continue..."
     continue
    fi

#    bin/200.sh $d
#    bin/300.sh $d
#    bin/400.sh $d
#    bin/500.sh $d
done

rm -rf $OUTDIR/$year-$month-$day
mkdir $OUTDIR/$year-$month-$day

echo $OUTDIR/$year-$month-$day/
echo find $INDIR -path ./_archive -prune -false -o -name model* -exec cp '{}' $OUTDIR/$year-$month-$day/ \;
find $INDIR -path ./_archive -prune -false -o -name 'model-*.stl' -exec cp '{}' $OUTDIR/$year-$month-$day/ \;

#!/usr/bin/bash

if [ "$#" -ne 1 ]; then
    echo "usage: mouthguard.sh <indir>"
    echo
    echo "  Executes process steps 200, 300, 400, and 500 for subdirectories"
    echo "  of <indir>."
    echo
    echo "  for example, mouthguard.sh ~/metaspace/PROD"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
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

    bin/200-trim.sh $d
    bin/300-convention.sh $d
    bin/400-normalize.sh $d
    bin/500-model-and-shell.sh $d

done


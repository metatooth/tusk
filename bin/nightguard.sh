#!/usr/bin/bash

if [ "$#" -ne 2 ]; then
    echo "usage: nightguard.sh <indir> <key>"
    echo
    echo "  Executes process steps 200 & 300 for subdirectories"
    echo "  of <indir>. Using the API <key>, will also store"
    echo "  300 output on s3://metatooth-cabinet."
    echo
    echo "  for example, nightguard.sh ~/metaspace/Nightguard \\"
    echo "                 2:8e09332323e586eab46a1a2b5ead12f5"
    exit
fi

INDIR=$1
KEY=$2

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

year=$(date --utc +%Y)
month=$(date --utc +%m)

for d in $INDIR/*; do
    bin/200.sh $d
    bin/300.sh $d

    bname=$(basename $d)
    check=$(md5sum $d/300/$bname.stl)

    arr=($check)

    aws s3 cp $d/300/$bname.stl s3://metatooth-cabinet/$year/$month/${arr[0]}.stl
    curl -i -v http://localhost:9393/assets \
	 -H 'Content-Type: application/json' \
	 -H 'Authorization: Metaspace-Token api_key='$KEY \
	 -d '{"data":{"name":"'$bname'","url":"https://metatooth-cabinet.s3.amazonaws.com/'$year'/'$month'/'${arr[0]}'.stl","mime_type":"application/sla"}}'
done



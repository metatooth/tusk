#!/usr/bin/bash

if [ "$#" -ne 3 ]; then
    echo "usage: nightguard.sh <indir> <url> <key>"
    echo
    echo "  Executes process steps 200 & 300 for subdirectories"
    echo "  of <indir>. Using the API <url> & <key>, will also store"
    echo "  300 output on s3://metatooth-cabinet."
    echo
    echo "  for example, nightguard.sh ~/metaspace/Nightguard \\"
    echo "                 http://localhost:9393 \\"
    echo "                 2:8e09332323e586eab46a1a2b5ead12f5"
    exit
fi

INDIR=$1
URL=$2
KEY=$3

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

year=$(date --utc +%Y)
month=$(date --utc +%m)
day=$(date --utc +%d)

for d in $INDIR/*; do
    bname=$(basename $d)

    if [[ $bname -eq "_archive" ]]; then
     continue
    fi

    bin/200.sh $d
    bin/300.sh $d
    bin/400.sh $d

    check=$(md5sum $d/400/$bname.stl)

    arr=($check)

    aws s3 cp $d/400/$bname.stl s3://metatooth-cabinet/$year/$month/$day/${arr[0]}.stl
    curl -v $URL/assets \
	 -H 'Content-Type: application/json' \
	 -H 'Authorization: Metaspace-Token api_key='$KEY \
	 -d '{"data":{"name":"'$bname'","url":"https://metatooth-cabinet.s3.amazonaws.com/'$year'/'$month'/'$day'/'${arr[0]}'.stl","mime_type":"application/sla"}}' \
   -o $d/400/curl.log
done



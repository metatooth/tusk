#!/usr/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <INDIR>"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

year=$(date --utc +%Y)
month=$(date --utc +%m)

for d in $INDIR/*; do
    #bin/200.sh $d
    #bin/300.sh $d

    bname=$(basename $d)
    check=$(md5sum $d/300/$bname.stl)

    arr=($check)

    #aws s3 cp $d/300/$bname.stl s3://metatooth-cabinet/$year/$month/${arr[0]}.stl
    curl -v http://localhost:9393/assets \
	 -H "Authorization: Metaspace-Token api_key=17:1d184d055f41a5b5402e7bc1ab45b85f" \
	 -d '{"data":{"name":"'$bname'","url":"https://metatooth-cabinet.s3.amazonaws.com/'$year'/'$month'/'${arr[0]}'.stl","mime_type":"application/sla"}}'
done



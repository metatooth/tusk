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
    if [[ "$bname" == "_archive" ]]; then
     echo "$bname is _archive"
     echo "Will continue..."
     continue
    fi

    #bin/200.sh $d
    #bin/300.sh $d
    #bin/400.sh $d

    check=$(md5sum $d/400/$bname.stl)
    arr=($check)

    cp $d/400/$bname.stl $d/400/${arr[0]}.stl

    stlname=$d/400/${arr[0]}.stl
    gltfname=$d/400/${arr[0]}.gltf
    binname=$d/400/${arr[0]}.bin

    ~/metaspace/assimp/bin/assimp export $stlname $gltfname
   
    s3key=$year/$month/$day/${arr[0]}
    bucket=metatooth-cabinet
    s3uri=s3://$bucket/$s3key
    url=https://$bucket.s3.amazonaws.com/$s3key.gltf

    aws s3 cp $stlname $s3uri.stl
    aws s3 cp $gltfname $s3uri.gltf
    aws s3 cp $binname $s3uri.bin

    body='{"data":{"name":"'$bname'","location":"'$url'","mime_type":"application.sla","service":"s3","bucket":"'$bucket'","s3key":"'$s3key'.gltf"}}'

    curl -v $URL/plans \
	 -H 'Content-Type: application/json' \
	 -H 'Authorization: Metaspace-Token api_key='$KEY \
	 -d $body \
   -o $d/400/curl.log
done


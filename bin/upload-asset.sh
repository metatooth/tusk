#!/usr/bin/bash

if [ "$#" -ne 3 ]; then
    echo "usage: upload-asset.sh <url> <key> <path>"
    echo
    echo "  Uploads an asset identified by <path> to the Metatooth Cabinet."
    echo "  Using the API <url> & <key>. STL only."
    echo
    echo "  for example, upload-asset.sh http://localhost:9393 \\"
    echo "                 2:8e09332323e586eab46a1a2b5ead12f5 \\"
    echo "                 /path/to/file.stl"
    exit
fi

apiurl=$1
apikey=$2
fullfilename=$3
filename=$(basename -- "$fullfilename")
extension="${filename##*.}"

stl=stl

if [ "${extension,,}" != "${stl,,}" ]; then
  echo "Only STL files supported at this time. Try again."
  exit
fi

year=$(date --utc +%Y)
month=$(date --utc +%m)
day=$(date --utc +%d)

s3key=$year/$month/$day/$filename
bucket=metatooth-cabinet
s3uri=s3://$bucket/$s3key
url=https://$bucket.s3.amazonaws.com/$s3key

aws s3 cp $fullfilename $s3uri

body='{"data":{"name":"'$filename'","url":"'$url'","mime_type":"application/sla","service":"s3","bucket":"'$bucket'","s3key":"'$s3key'"}}'

echo $body

curl -v $apiurl/assets \
  -H 'Content-Type: application/json' \
	-H 'Authorization: Metaspace-Token api_key='$apikey \
	-d $body


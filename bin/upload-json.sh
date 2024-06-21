#!/usr/bin/bash

if [ "$#" -ne 3 ]; then
    echo "usage: upload-json.sh <stl> <url> <key>"
    echo
    echo "  Creates a JSON file for <stl> and using the API <url> & <key>"
    echo "  stores on s3://metatooth-cabinet."
    echo
    echo "  for example, upload-json.sh /path/to/stl \\"
    echo "                 http://localhost:9393 \\"
    echo "                 26:b0a1b84a6e591897d4249990f928f1f3"
    exit
fi

STLPATH=$1
URL=$2
KEY=$3

if [ ! -e "$STLPATH" ]; then
   echo "$STLPATH does not exist. Try again."
   exit
fi

extension=json
uri=/plans
urlname=location
mimetype=application/json

year=$(date --utc +%Y)
month=$(date --utc +%m)
day=$(date --utc +%d)

bname=$(basename $STLPATH)

check=$(md5sum $STLPATH)
arr=($check)

cp $STLPATH /tmp/${arr[0]}.stl

stlname=/tmp/${arr[0]}.stl
jsonname=/tmp/${arr[0]}.json
  
s3key=$year/$month/$day/${arr[0]}
bucket=metatooth-cabinet
s3uri=s3://$bucket/$s3key
urlstub=https://$bucket.s3.amazonaws.com/$s3key
url=$urlstub.$extension

aws s3 cp $stlname $s3uri.stl

cat > $jsonname <<EOF
{
  "__metadata__": {
    "format": "assimp2json"
   ,"version": 100
  },"rootnode": {
    "name": "<MetatoothRoot>"
   ,"transformation": [ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ]
   ,"children": [
      {
        "name": "maxillary"
       ,"transformation": [ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ]
       ,"meshrefs": [ 0 ]
      }
    ]
  }
  ,"meshes": [
  ]
  ,"meshrefs": [
    {
       "name": "stl"
      ,"materialindex": 0
      ,"url": "$urlstub.stl"
    }
  ]
  ,"materials": [
    {
      "properties": [
        {
					 "key": "?mat.name"
					,"semantic": 0
					,"index": 0
					,"type": 3
					,"value": "DefaultMaterial"
				}
				,{
					 "key": "?clr.diffuse"
					,"semantic": 0
					,"index": 0
					,"type": 1
					,"value": [
						 0
						,0.866666
						,0.466666
						,0
					]
				}
				,{
					 "key": "?clr.specular"
					,"semantic": 0
					,"index": 0
					,"type": 1
					,"value": [
						 0.133333
						,0.133333
						,0.133333
						,0
					]
				}
				,{
					 "key": "?mat.shininess"
					,"semantic": 0
					,"index": 0
					,"type": 1
					,"value": 40
				}
      ]
    }
  ]
}
EOF

aws s3 cp $jsonname $s3uri.json

body='{"data":{"name":"'$bname'","'$urlname'":"'$url'","mime_type":"'$mimetype'","service":"s3","bucket":"'$bucket'","s3key":"'$s3key'.'$extension'"}}'

curl $URL$uri \
     -H 'Content-Type: application/json' \
     -H 'Authorization: Metaspace-Token api_key='$KEY \
     -d $body \
     -o /tmp/curl.log
   
cat /tmp/curl.log


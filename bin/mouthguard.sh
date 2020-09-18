#!/usr/bin/bash

if [ "$#" -ne 4 ]; then
    echo "usage: mouthguard.sh <indir> <url> <key> <type>"
    echo
    echo "  Executes process steps 200, 300, and 400 for subdirectories"
    echo "  of <indir>. Using the API <url> & <key>, stores 400 stage"
    echo "  on s3://metatooth-cabinet. Specify ASSET or PLAN for <type>."
    echo
    echo "  for example, mouthguard.sh ~/metaspace/Mouthguard \\"
    echo "                 http://localhost:9393 \\"
    echo "                 26:b0a1b84a6e591897d4249990f928f1f3 \\"
    echo "                 PLAN"
    exit
fi

INDIR=$1
URL=$2
KEY=$3
TYPE=$4

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

if [ "$TYPE" == "ASSET" ]; then                        
  extension=stl
  uri=/assets
  urlname=url
  mimetype=application/sla
elif [ "$TYPE" == "PLAN" ]; then
  extension=json
  uri=/plans
  urlname=location
  mimetype=application/json
else
  echo "$TYPE not in [ASSET, PLAN]. Try again."
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

    bin/200.sh $d
    bin/300.sh $d
    bin/400.sh $d

    check=$(md5sum $d/400/$bname.stl)
    arr=($check)

    cp $d/400/$bname.stl $d/400/${arr[0]}.stl

    stlname=$d/400/${arr[0]}.stl
    jsonname=$d/400/${arr[0]}.json
    binname=$d/400/${arr[0]}.bin
  
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
  ,"flags": 0
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
						,0.733333
						,0.933333
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
   -o $d/400/curl.log
   
   cat $d/400/curl.log
   echo
done


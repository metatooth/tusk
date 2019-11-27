#!/bin/sh

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <SOURCESTL> <BASEDIR> <NUM>"
    exit
fi

SOURCESTL=$1
BASEDIR=$2
NUM=$3

rm -rf $BASEDIR/$NUM
mkdir -p $BASEDIR/$NUM/000

cp $SOURCESTL $BASEDIR/$NUM/000/$NUM.stl


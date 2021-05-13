#!/usr/bin/bash

if [ "$#" -ne 1 ]; then
    echo "usage: mouthguard.sh <indir>"
    echo
    echo "  Generates a mouthguard for <indir>."
    echo "  Assumes <indir>/0000/100-marked.ply exists."
    echo
    echo "  for example, mouthguard.sh ~/metaspace/PROD/0000"
    exit
fi

INDIR=$1

if [ ! -d "$INDIR" ]; then
   echo "$INDIR does not exist. Try again."
   exit
fi

MSHLB=~/metaspace/meshlab/distrib/meshlabserver

LOG=$INDIR/meshlab.log 

$MSHLB -l $LOG -i $INDIR/100-marked.ply -o $INDIR/200-trimmed.ply -m vn fn -s etc/select-faces-by-color.mlx

_build/bin/tusk/tusk convention $INDIR/200-trimmed.ply $INDIR/300-oriented.ply

_build/bin/tusk/tusk normalize $INDIR/300-oriented.ply $INDIR/400-normalized.ply

_build/bin/tusk/tusk extrude 11 $INDIR/400-normalized.ply $INDIR/500-extruded.ply
$MSHLB -l $LOG -i $INDIR/500-extruded.ply -o $INDIR/505-open.ply -m vn fn -s etc/delete-base-plane.mlx

$MSHLB -l $LOG -i $INDIR/500-extruded.ply -o $INDIR/505-merged.ply -m vn fn -s etc/merge-close-vertices.mlx
$MSHLB -l $LOG -i $INDIR/505-merged.ply -o $INDIR/510-cleaned.ply -m vn fn -s etc/cleaning--topology-filters.mlx
$MSHLB -l $LOG -i $INDIR/510-cleaned.ply -o $INDIR/520-model.ply -m vn fn -s etc/cleaning--critical.mlx

$MSHLB -l $LOG -i $INDIR/400-normalized.ply -o $INDIR/530-shell.ply -m vn fn -s etc/500-shell.mlx

_build/bin/tusk/tusk subtract $INDIR/530-shell.ply $INDIR/520-model.ply $INDIR/600-mouthguard.ply


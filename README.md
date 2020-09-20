# Getting Started

## Get and build Meshlab

```
$ sudo apt install libgl1-mesa-dev
$ sudo apt install qt5-default
$ sudo apt install libqt5xmlpatterns5 libqt5xmlpatterns5-dev
$ sudo apt install qtscript5-dev qtdeclarative5-dev
$ sudo apt install jq cmake curl
$ mkdir ~/metaspace
$ cd metaspace
$ curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
$ unzip awscliv2.zip
$ sudo ./aws/install
$ aws configure
$ git clone --recursive https://github.com/cnr-isti-vclab/meshlab
$ cd meshlab
$ cmake src/
$ make
```

# mouthguard.sh

Processes steps 200-400 for a directory.

# Process Steps

## 000

Manual step: Starting STL file

## 100

Manual step: Z-Paint the faces to be deleted (black). Save as PLY format.

## 200

Automated step: Execute '200.sh /path/to/folder/af00'

## 300

Automated step

## 400

Automated step

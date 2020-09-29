# Getting Started

## Ubuntu 20.04

Instructions are for an Ubuntu workstation.

## Build tools

```
$ sudo apt install clang-tidy
```

## Install CGAL with Qt5

```
$ sudo apt install libcgal-dev libcgal-qt5-dev libqt5svg5-dev
```

## Get and build Meshlab

```
$ sudo apt install libgl1-mesa-dev
$ sudo apt install qt5-default
$ sudo apt install libqt5xmlpatterns5 libqt5xmlpatterns5-dev
$ sudo apt install qtscript5-dev qtdeclarative5-dev
$ sudo apt install jq cmake curl
$ mkdir ~/metaspace
$ cd metaspace
$ git clone --recursive https://github.com/cnr-isti-vclab/meshlab
$ cd meshlab
$ cmake src/
$ make
```

## Add AWS CLI

```
$ curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
$ unzip awscliv2.zip
$ sudo ./aws/install
$ aws configure
```

## Get Code

```
$ cd metaspace
$ git clone https://github.com/metatooth/mouthguard.git
$ cd mouthguard
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

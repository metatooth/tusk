# Tusk

Make teeth stuff.

## Getting Started

Using Ubuntu 22.04

```
$ sudo apt install clang-tidy
$ sudo apt install libcgal-dev libcgal-qt5-dev libqt5svg5-dev
$ sudo apt install libgl1-mesa-dev
$ sudo apt install qt5-default
$ sudo apt install libqt5xmlpatterns5 libqt5xmlpatterns5-dev
$ sudo apt install qtscript5-dev qtdeclarative5-dev
$ sudo apt install jq cmake curl
$ sudo apt install cppcheck
$ git clone https://github.com/metatooth/tusk.git
```

## Build Setup

### Build Tusk

```
$ cd tusk
$ mkdir _build
$ cd _build
$ cmake ../src
$ make
```

### Add AWS CLI

```
$ curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
$ unzip awscliv2.zip
$ sudo ./aws/install
$ aws configure
```

### Install and Configure ejabberd

https://www.digitalocean.com/community/tutorials/how-to-install-ejabberd-xmpp-server-on-ubuntu

https://computingforgeeks.com/install-ejabberd-xmpp-server-on-ubuntu/#ex1

## License

Copyright 2022 Metatooth LLC. See the [LICENSE](LICENSE).

# Getting Started

''
$ sudo apt install libgl1-mesa-dev
$ sudo apt install qt5-default libxml5patterns5 libxml5patterns5-dev qtscript5-dev
$ mkdir ~/metaspace
$ cd metaspace
$ git clone --recursive https://github.com/cnr-isti-vclab/meshlab
$ cd meshlab
$ cmake src/
$ make
''

# 000

STL files of scans.

# 100

Manual step: Z-Paint the faces to be deleted (black). Save as PLY format.

# 200

Automated step: Execute '200.sh /path/to/folder/af00'

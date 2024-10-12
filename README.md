# Map Le_Creusot

[![Documentation](https://img.shields.io/badge/Map-documentation-brightgreen.svg)](https://github.com/DJNing/MapLeCreusot/blob/master/Documentations/Report/Report.pdf)

[![GitHub license](https://img.shields.io/badge/license-GNU3-blue.svg)](/LICENSE)

>C++  project using Qt GUI depending on [Libosmium](https://github.com/osmcode/libosmium),and [Boost Graph Libray](https://www.boost.org/doc/libs/1_72_0/libs/graph/doc/table_of_contents.html) Libraries.

## Authors
1. **Deng Jianning**
2. **Belal Hmedan**

## Supported Platforms
 - Linux (Ubuntu16+)


## Dependencies

1.  **Boost** has to be installed if you don’t have it. Follow the installation guide below.

- Windows
Using vcpkg from Microsoft (https://github.com/microsoft/vcpkg) you can install
Boost libraries through command line window as following:

`vcpkg install boost`.

- Ubuntu
If it is not pre-install in your Ubuntu, a single command should help you do this (for
Ubuntu 18.04):

`$ sudo apt-get install libboost-all-dev`

2. **Libosmium** This is a header-only library, no need for installation. Just includ the header files in the
project is OK to go. This is already put in the project repository.

3. **Protozero** This is a header-only library, no need for installation. Just includ the header files in the
project is OK to go. This is already put in the project repository.

4. **Zlib** has to be installed if you don’t have it. Follow the installation guide below.

- Windows

`vcpkg install zlib`

- Ubuntu

`$ sudo apt-get install zlib1g-dev`

5. **Expat** has to be installed if you don’t have it. Follow the installation guide below.

- Windows

`vcpkg install expat`

- Ubuntu

`$ apt−get insatll expat`

6. PThread ( for Linux ), Use the following commands to install the library 

`$ sudo apt-get install libpthread-stubs0-dev`

`$ sudo apt-get install expat`

## Known Issues

- Crashes when reloading a new OSM file

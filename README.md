# Map Le_Creusot

[![GitHub license](https://img.shields.io/badge/license-GNU3-blue.svg)](/LICENSE)
> Map of [Le Creusot / France](https://en.wikipedia.org/wiki/Le_Creusot) in C++ using:
> -  [Qt](https://www.qt.io/)
> - [Libosmium](https://github.com/osmcode/libosmium)
> - [Boost Graph Libray](https://www.boost.org/doc/libs/1_72_0/libs/graph/doc/table_of_contents.html)


## Dependencies

```sh
sudo apt-get install libboost-all-dev
sudo apt-get install zlib1g-dev
sudo apt-get install libpthread-stubs0-dev
sudo apt-get install expat
sudo apt install libprotozero-dev
sudo apt install libosmium2-dev
```

## Installation
```sh
mkdir build && cd build
# future plan to support cmake
qmake ..
make -j$(nproc)
```

## Usage
```sh
# inside build directory
./map
```

![map](./media/map.gif)

## Authors

1. **[Deng Jianning](https://www.linkedin.com/in/jianningdeng)**
2. **[Belal Hmedan](https://www.linkedin.com/in/belalhmedan)**


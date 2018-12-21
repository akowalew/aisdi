# AISDI

This is a set of projects written during AISDI classes at Warsaw University of Technology. The aim of project was to develop some basic structures and algorithms using C++.

The whole project is written using modern C++. Build scripts are written in CMake. Unit tests are developed using Boost.Test framework. Profiling may be done using Hayai benchmark framework.

## How to build

Type following in the terminal:

```sh
	git submodule init
	git submodule update
```

and then

```sh
	mkdir build-debug
	cd build-debug
	cmake .. -DCMAKE_BUILD_TYPE=Debug
	make -j5
```

## How to run test

```sh
	make test
```

For more details, look into subprojects folders: `linear`, `maps`, `graphs`

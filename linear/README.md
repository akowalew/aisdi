# AISDI - linear

This project contains implementation of `Vector` and `List` classes. The first one is a sequence container that encapsulates dynamic size arrays. The second is a container that supports constant time insertion and removal of elements from anywhere in the container. Interfaces of these classes are similar to containers found in `std` C++ library. Project is tested both with GCC (at least 6.3.0) and Clang (at least 3.8.1). Both classes have unit tests written with Boost Unit Test Framework and some benchmarks supported by Hayai framework.

## How to run test

There are four tests modules, two for `Vector`, and two for `List`. To run all of them:

```sh
	make test
```

To run separately:

```sh
	./test/VectorTest
	./test/ListTest
	./test/VectorTest2
	./test/ListTest2
```

## How to run benchmarks

There are two benchmarks modules, which may be run by typing:

```sh
	./test/VectorBenchmark
	./test/ListBenchmark
```

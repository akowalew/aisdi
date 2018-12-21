# AISDI - maps

This project contains implementation of `TreeMap` and `HashMap` classes. 

The first one is a binary search tree. It does not do any rebalancing, since it is not an AVL tree. 
The second one is a standard hash map, which is built on dynamic array of lists. It is using components written in `linear` library: `aisdi::Vector<T>` and `aisdi::List<T>`. Hash algorithm used by default is `std::hash<T>` (which may be overrided with template parameters. 
Both containers provides interfaces similar to classes found in `std` C++ library: `std::map<Key, T>` and `std::unordered_map<Key, T>`. Both classes have unit tests written with Boost Unit Test Framework and some benchmarks supported by Hayai framework.

## How to run test

There are two tests modules, one for `TreeMap`, and one for `HashMap`. To run all of them:

```sh
	make test
```

## How to run benchmarks

There are two benchmarks modules, which may be run by typing (be sure to compile project in `Release` mode first):

```sh
	./test/TreeMapBenchmark
	./test/HashMapBenchmark
```

# A small wrapper around few of the more common Mpi C-functions


## Overview


Although, the library is intended to be as dependency-free as possible, the following libraries are still required:

- C++17 (Will probably compile with C++14 without the unittests)
- [CMake](https://cmake.org/) build script for cross-platform configuration (see 
  [here](#cmakeliststxt) for more info)
- [Open MPI](https://www.open-mpi.org/) for distributed memory parallelization

In addition the following libraries are used but they ship alongside the library

- [Catch](https://github.com/catchorg/Catch2) unit testing framework (see 
  [here](#mytestscpp))

The following tools are also used but are not mandatory

- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) for automated 
  source code formatting (see [here](#clang-format))
- [Clang-Tidy](http://clang.llvm.org/extra/clang-tidy/) for static analysis to 
  catch coding errors (see [here](#clang-tidy))




### Linux

```bash
$ mkdir build
$ cd build
$ cmake ..
```

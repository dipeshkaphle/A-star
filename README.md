# Instructions for running code

- There are 3 C++ files that should be run for all the plots to be available.
  `all_A_stars.cpp` , `profile_operations.cpp` and `profile_joins.cpp`

## Compiling with g++/clang

> In windows, open powershell and navigate to the directory in which the code is
> located. In linux, open terminal and navigate to the directory in which the
> code is located.

- `g++ -O3 -o all_A_stars all_A_stars.cpp` to compile `all_A_stars.cpp` and
  `./all_A_stars` to run it.
- `g++ -O3 -o profile_operations profile_operations.cpp` to compile
  `profile_operations.cpp` and `./profile_operations` to run it.
- `g++ -O3 -o profile_joins profile_joins.cpp` to compile `profile_joins.cpp`
  and `./profile_joins` to run it.
- `g++ -O3 -o demo demo.cpp` to compile `demo.cpp` and `./demo` to run it.

## Using CMake and Make

- You must have `Cmake` and `make` installed for this.

  - `cmake .`
  - `make all` to build all the binaries
  - Run the binaries as you wish. The instruction for plotting will be printed
    at the end when you run the binaries.

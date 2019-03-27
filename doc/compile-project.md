| Executables | Description                           |  
|:------------|:--------------------------------------|
| `bwtzip`    | Original sequential version of bwtzip |
| `bwtunzip`  | Decompressor                          |
| `pbwtzip`   | Parallel version (3-stage pipeline)   |
| `pbwtzip2`  | Parallel version (2-stage pipeline)   |
| `tests`     | Test to check pbwtzip and pbwtzip2    |


# Compile project

## Debug build

Fast way:

```bash
$ chmod +x ./script/*

$ ./script/build_debug.sh
```

or, manually:

```bash
$ mkdir cmake-build-debug && cd cmake-build-debug

$ cmake -DCMAKE_BUILD_TYPE=Debug ..

$ make -j4
```

## Release build

Fast way:

```bash
$ chmod +x ./script/*

$ ./script/build_release.sh
```

or, manually:

```bash
$ mkdir cmake-build-release && cd cmake-build-release

$ cmake -DCMAKE_BUILD_TYPE=Release ..

$ make -j4
```

# Run tests

```bash
$ cd cmake-build-release

$ ctest --verbose
```

## Test passing

```
ctest --verbose
UpdateCTestConfiguration  from :/bwtzip-openmp/cmake-build-debug/DartConfiguration.tcl
Parse Config file:/bwtzip-openmp/cmake-build-debug/DartConfiguration.tcl
UpdateCTestConfiguration  from :/bwtzip-openmp/cmake-build-debug/DartConfiguration.tcl
Parse Config file://bwtzip-openmp/cmake-build-debug/DartConfiguration.tcl
Test project /bwtzip-openmp/cmake-build-debug
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: tests

1: Test command: /bwtzip-openmp/cmake-build-debug/tests
1: Test timeout computed to be: 1500
1: [TEST][pbwtzip]-------- Compressed file matches original file ---------
1: [TEST][pbwtzip] Compressing ../test/data_sample.txt to ./pbwtzip_data_sample_compressed.bwtzip
1: ./pbwtzip_data_sample_compressed.bwtzip,4.1.1,409600,11,1,6,1,1,2,0.0547315,0.533713,0.273296,0.137589,0.0073339,6.47994
1: [TEST][pbwtzip] Decompressing ./pbwtzip_data_sample_compressed.bwtzip to ./pbwtzip_data_sample_decompressed.txt
1: [TEST][pbwtzip] Testing match of original and decompressed file
1: [TEST][pbwtzip2]-------- Compressed file matches original file ---------
1: [TEST][pbwtzip2] Compressing ../test/data_sample.txt to ./pbwtzip2_data_sample_compressed.bwtzip
1: ./pbwtzip2_data_sample_compressed.bwtzip,4.2,409600,10,1,6,2,1,0.0941392,0.683185,0.328479,0.01135,7.18643
1: [TEST][pbwtzip2] Decompressing ./pbwtzip2_data_sample_compressed.bwtzip to ./pbwtzip2_data_sample_decompressed.txt
1: [TEST][pbwtzip2] Testing match of original and decompressed file
1: ===============================================================================
1: All tests passed (2 assertions in 2 test cases)
1: 
1/1 Test #1: tests ............................   Passed   21.11 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =  21.11 sec
```

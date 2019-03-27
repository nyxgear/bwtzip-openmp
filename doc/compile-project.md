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

Manually:

```bash
$ mkdir cmake-build-debug

$ cd cmake-build-debug

$ cmake -DCMAKE_BUILD_TYPE=Debug ..

$ make -j4
```

## Release build

Fast way:

```bash
$ chmod +x ./script/*

$ ./script/build_release.sh
```

Manually:

```bash
$ mkdir cmake-build-release

$ cd cmake-build-release

$ cmake -DCMAKE_BUILD_TYPE=Release ..

$ make -j4
```


#!/bin/bash
# This script download the files for the large source code corpus.

# https://nuwen.net/bwtzip.html#corpus

# Large Source Code Corpus:
# gcc-3.2.1.tar    - gcc 3.2.1 source code
# linux-2.4.20.tar - Linux kernel 2.4.20 source code

# Large Source Code Corpus URLs
declare -a urls=(
    "https://ftp.gnu.org/gnu/gcc/gcc-3.2.1/gcc-3.2.1.tar.gz"
    "https://mirrors.edge.kernel.org/pub/linux/kernel/v2.4/linux-2.4.0.tar.gz"
)


set -e

ROOT=`git rev-parse --show-toplevel`

echo "==> Downloading files of LARGE SOURCE CODE CORPUS..."
for url in "${urls[@]}"; do
    filename=${url##*/}
    if [ ! -f $ROOT/dataset/large_source_code/$filename ]; then
        echo "==> Downloading from $url ..."
        wget "$url" -P $ROOT/dataset/large_source_code
    else
        echo "==> $ROOT/dataset/large_source_code/$filename found. Download skipped."
    fi
done
echo -e "==> Done.\n"


declare -a directories=(
    "gcc-3.2.1"
    "linux-2.4.0"
)


echo "==> Unpacking archives of LARGE SOURCE CODE CORPUS..."
for dir in "${directories[@]}"; do
    if [ ! -d $ROOT/dataset/large_source_code/$dir ]; then
        echo "==> Extracting ${dir}.tar.gz ..."
        tar -zxvf "$ROOT/dataset/large_source_code/$dir.tar.gz" -C $ROOT/dataset/large_source_code/
    else
        echo "==> $ROOT/dataset/large_source_code/$dir found. Unpacking skipped."
    fi
done

# patch linux folder name
if [ -d "$ROOT/dataset/large_source_code/linux" ]; then
        mv "$ROOT/dataset/large_source_code/linux" "$ROOT/dataset/large_source_code/linux-2.4.0"
fi
echo -e "==> Done.\n"
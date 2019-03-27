#!/bin/bash
# The script downloads the require files to perform the speedup analysis

declare -a urls=(
    "https://www.gutenberg.org/files/161/old/sense11.txt"
    "https://www.gutenberg.org/files/1400/1400-0.txt"      # instead of unavailable grexp10.txt
    "http://www.gutenberg.org/cache/epub/1023/pg1023.txt"  # instead of unavailable blkhs10.txt
    "https://www.gutenberg.org/files/6344/old/world02.txt"
    "https://www.gutenberg.org/files/4546/old/cwgen11.txt"
    "https://www.gutenberg.org/files/5050/old/suall10.txt"
    "https://www.gutenberg.org/files/3200/old/mtent13.txt"
    "http://gutenberg.nust.na/gutenberg/etext02/01hgp10a.txt"
)

set -e

ROOT=`git rev-parse --show-toplevel`

echo "==> Downloading required files to run speedup analysis..."
for url in "${urls[@]}"; do
    filename=${url##*/}
    if [ ! -f $ROOT/dataset/speedup_analysis_files/$filename ]; then
        echo "==> Downloading from $url ..."
        wget "$url" -P $ROOT/dataset/speedup_analysis_files
    else
        echo "==> $ROOT/dataset/speedup_analysis_files/$filename found. Download skipped."
    fi
done
echo -e "==> Done.\n"
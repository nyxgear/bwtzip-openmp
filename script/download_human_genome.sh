#!/bin/bash
# This script download the files for the human genome corpus.

# https://nuwen.net/bwtzip.html#corpus

# Human Genome Corpus:
# 01hgp10a.txt - The Human Genome, Chromosome 1
# 02hgp10a.txt - The Human Genome, Chromosome 2
# 03hgp10a.txt - The Human Genome, Chromosome 3
# 04hgp10a.txt - The Human Genome, Chromosome 4
# 05hgp10a.txt - The Human Genome, Chromosome 5
# 06hgp10a.txt - The Human Genome, Chromosome 6
# 07hgp10a.txt - The Human Genome, Chromosome 7
# 08hgp10a.txt - The Human Genome, Chromosome 8
# 09hgp10a.txt - The Human Genome, Chromosome 9
# 0xhgp10a.txt - The Human Genome, Chromosome X
# 0yhgp10a.txt - The Human Genome, Chromosome Y
# 10hgp10a.txt - The Human Genome, Chromosome 10
# 11hgp10a.txt - The Human Genome, Chromosome 11
# 12hgp10a.txt - The Human Genome, Chromosome 12
# 13hgp10a.txt - The Human Genome, Chromosome 13
# 14hgp10a.txt - The Human Genome, Chromosome 14
# 15hgp10a.txt - The Human Genome, Chromosome 15
# 16hgp10a.txt - The Human Genome, Chromosome 16
# 17hgp10a.txt - The Human Genome, Chromosome 17
# 18hgp10a.txt - The Human Genome, Chromosome 18
# 19hgp10a.txt - The Human Genome, Chromosome 19
# 20hgp10a.txt - The Human Genome, Chromosome 20
# 21hgp10a.txt - The Human Genome, Chromosome 21
# 22hgp10a.txt - The Human Genome, Chromosome 22

# Human Genome files
declare -a human_genome_files=(
    "01hgp10a.txt"
    "02hgp10a.txt"
    "03hgp10a.txt"
    "04hgp10a.txt"
    "05hgp10a.txt"
    "06hgp10a.txt"
    "07hgp10a.txt"
    "08hgp10a.txt"
    "09hgp10a.txt"
    "0xhgp10a.txt"
    "0yhgp10a.txt"
    "10hgp10a.txt"
    "11hgp10a.txt"
    "12hgp10a.txt"
    "13hgp10a.txt"
    "14hgp10a.txt"
    "15hgp10a.txt"
    "16hgp10a.txt"
    "17hgp10a.txt"
    "18hgp10a.txt"
    "19hgp10a.txt"
    "20hgp10a.txt"
    "21hgp10a.txt"
    "22hgp10a.txt"
)

set -e

BASE_URL="http://gutenberg.nust.na/gutenberg/etext02/"
ROOT=$(git rev-parse --show-toplevel)

echo "==> Downloading files of HUMAN GENOME CORPUS..."
for filename in "${human_genome_files[@]}"; do
    if [ ! -f $ROOT/dataset/human_genome/$filename ]; then
        url="${BASE_URL}/${filename}"
        echo "==> Downloading from $url ..."
        wget "$url" -P $ROOT/dataset/human_genome
    else
        echo "==> $ROOT/dataset/human_genome/$filename found. Download skipped."
    fi
done
echo -e "==> Done.\n"

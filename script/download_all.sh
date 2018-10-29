#!/bin/bash

# The script downloads the three large data set described at https://nuwen.net/bwtzip.html#corpus

CURR_DIR=$(dirname $(readlink -f $0))

"$CURR_DIR/download_human_genome.sh"
"$CURR_DIR/download_large_english_text.sh"
"$CURR_DIR/download_large_source_code.sh"

#!/bin/bash
# This script download the files for the large english text corpus.

# https://nuwen.net/bwtzip.html#corpus

# Large English Text Corpus:
# 1dfre10.txt  - The History of The Decline and Fall of the Roman Empire, Volume 1 by Edward Gibbon
# 2000010a.txt - 20,000 Leagues Under the Sea by Jules Verne
# 2city12.txt  - A Tale of Two Cities by Charles Dickens
# 2dfre11.txt  - The History of The Decline and Fall of the Roman Empire, Volume 2 by Edward Gibbon
# 3dfre10.txt  - The History of The Decline and Fall of the Roman Empire, Volume 3 by Edward Gibbon
# 4dfre10.txt  - The History of The Decline and Fall of the Roman Empire, Volume 4 by Edward Gibbon
# 5dfre11.txt  - The History of The Decline and Fall of the Roman Empire, Volume 5 by Edward Gibbon
# 6dfre10.txt  - The History of The Decline and Fall of the Roman Empire, Volume 6 by Edward Gibbon
# 7crmp10.txt  - Crime and Punishment by Fyodor Dostoyevsky
# blkhs10.txt  - Bleak House by Charles Dickens
# cnmmm10.txt  - On the Economy of Machinery and Manufacture by Charles Babbage
# cprfd10.txt  - David Copperfield by Charles Dickens
# cwgen11.txt  - The Memoirs of Ulysses S. Grant, William T. Sherman, and Philip H. Sheridan
# dosie10.txt  - Reflections on the Decline of Science in England by Charles Babbage
# dscmn10.txt  - The Descent of Man by Charles Darwin
# dyssy10b.txt - The Odyssey of Homer, translated by Alexander Pope
# feder16.txt  - The Federalist Papers
# flat10a.txt  - Flatland: A Romance of Many Dimensions by Edwin Abbott Abbott
# grexp10.txt  - Great Expectations by Charles Dickens
# grimm10a.txt - Household Tales by The Brothers Grimm
# iliab10.txt  - The Iliad of Homer, translated by Lang, Leaf, and Myers
# jargon.html  - The Jargon File 4.3.3
# lnent10.txt  - The Entire Writings of Abraham Lincoln
# lwmen10.txt  - Little Women by Louisa May Alcott
# mtent13.txt  - The Entire Writings of Mark Twain
# nkrnn11.txt  - Anna Karenina by Leo Tolstoy, translated by Constance Garnett
# otoos610.txt - On the Origin of Species, 6th Edition, by Charles Darwin
# pandp12.txt  - Pride and Prejudice by Jane Austen
# pwprs10.txt  - The Pickwick Papers by Charles Dickens
# sense11.txt  - Sense and Sensibility by Jane Austen
# slstr10.txt  - Sidelights on Astronomy and Kindred Fields of Popular Science by Simon Newcomb
# suall10.txt  - All State of the Union Addresses up to January 29, 2002
# tmwht10.txt  - The Man Who Was Thursday by G. K. Chesterton
# trabi10.txt  - Theodore Roosevelt, An Autobiography
# twtp110.txt  - The Writings of Thomas Paine, Volume 1
# twtp210.txt  - The Writings of Thomas Paine, Volume 2
# vbgle11a.txt - A Naturalist's Voyage Round the World by Charles Darwin
# warw11.txt   - The War of the Worlds by H. G. Wells
# world02.txt  - The 2002 CIA World Factbook
# wrnpc10.txt  - War and Peace by Leo Tolstoy


declare -a urls=(
    "https://www.gutenberg.org/files/731/old/1dfre10.txt"
    "https://www.gutenberg.org/files/164/old/2000010.txt"
    "https://www.gutenberg.org/files/98/old/2city12.txt"
    "https://www.gutenberg.org/cache/epub/891/pg891.txt"   # instead of unavailable 2dfre11.txt
    "https://www.gutenberg.org/files/733/old/3dfre10.txt"
    "https://www.gutenberg.org/cache/epub/893/pg893.txt"   # instead of unavailable 4dfre10.txt
    "https://www.gutenberg.org/files/735/old/5dfre11.txt"
    "https://www.gutenberg.org/files/736/old/6dfre10.txt"
    "https://www.gutenberg.org/files/2554/old/7crmp10.txt"
    "http://www.gutenberg.org/cache/epub/1023/pg1023.txt"  # instead of unavailable blkhs10.txt
    "http://www.gutenberg.org/cache/epub/4238/pg4238.txt"  # instead of unavailable cnmmm10.txt
    "http://www.gutenberg.org/files/766/766-0.txt"         # instead of unavailable cprfd10.txt
    "https://www.gutenberg.org/files/4546/old/cwgen11.txt"
    "https://www.gutenberg.org/files/1216/old/dosie10.txt"
    "https://www.gutenberg.org/files/2300/old/dscmn10.txt"
    "https://www.gutenberg.org/cache/epub/3160/pg3160.txt" # instead of unavailable dyssy10b.txt
    "https://www.gutenberg.org/cache/epub/18/pg18.txt"     # instead of unavailable feder16.txt
    "https://www.gutenberg.org/files/201/old/flat10a.txt"
    "https://www.gutenberg.org/files/1400/1400-0.txt"      # instead of unavailable grexp10.txt
    "https://www.gutenberg.org/cache/epub/5314/pg5314.txt" #  instead of unavailable grimm10a.txt
    "https://www.gutenberg.org/cache/epub/6130/pg6130.txt" #  instead of unavailable iliab10.txt
    "https://jargon-file.org/archive/jargon-4.3.3.dos.txt"
    "https://www.gutenberg.org/cache/epub/2656/pg2656.txt" #  instead of unavailable lnent10.txt
    "https://www.gutenberg.org/files/514/old/lwmen10.txt"
    "https://www.gutenberg.org/files/3200/old/mtent13.txt"
    "https://www.gutenberg.org/files/1399/old/nkrnn11.txt"
    "https://www.gutenberg.org/files/2009/old/otoos610.txt"
    "https://www.gutenberg.org/files/1342/old/pandp12.txt"
    "https://www.gutenberg.org/files/580/old/pwprs10.txt"
    "https://www.gutenberg.org/files/161/old/sense11.txt"
    "https://www.gutenberg.org/files/4065/old/slstr10.txt"
    "https://www.gutenberg.org/files/5050/old/suall10.txt"
    "https://www.gutenberg.org/files/1695/old/tmwht10.txt"
    "https://www.gutenberg.org/files/3335/old/trabi10.txt"
    "https://www.gutenberg.org/files/4943/old/twtp110.txt"
    "https://www.gutenberg.org/files/3742/old/twtp210.txt"
    "https://www.gutenberg.org/files/3704/old/vbgle11a.txt"
    "https://www.gutenberg.org/files/36/old/warw11.txt"
    "https://www.gutenberg.org/files/6344/old/world02.txt"
    "https://www.gutenberg.org/files/2600/old/wrnpc10.txt"
)

set -e

ROOT=`git rev-parse --show-toplevel`

echo "==> Downloading files of LARGE ENGLISH TEXT CORPUS..."
for url in "${urls[@]}"; do
    filename=${url##*/}
    if [ ! -f $ROOT/dataset/large_english_text/$filename ]; then
        echo "==> Downloading from $url ..."
        wget "$url" -P $ROOT/dataset/large_english_text
    else
        echo "==> $ROOT/dataset/large_english_text/$filename found. Download skipped."
    fi
done
echo -e "==> Done.\n"
#! /bin/bash
#
# script.sh
# Copyright (C) 2020 rick <ckyiricky@gmail.com>
#
# Distributed under terms of the MIT license.
#

sudo apt update
sudo apt install g++ -y
g++ -std=c++17 better_bw_matching.cpp -o bw_matching -O3
g++ -std=c++17 suffix_array_matching.cpp -o suffix_matching -O3
g++ -std=c++17 suffix_array_mismatch.cpp -o suffix_mismatch -O3
for X in 10000 50000 100000
do
    for Y in 10 100
    do
        echo "Better BW matching with $X reads of length $Y"
        ./bw_matching "./data/data_$X"_"$Y" > "output_bw_matching_$X"_"$Y"
        echo "Suffix array matching with $X reads of length $Y"
        ./suffix_matching "./data/data_$X"_"$Y" > "output_suffix_matching_$X"_"$Y"
    done
done

for D in 1 2 3
do
    echo "Suffix array tolarent matching with 10000 reads of length 10 at most $D mismatches"
    ./suffix_mismatch ./data/data_10000_10 $D > "output_suffix_mismatch_10000_10_$D"
done

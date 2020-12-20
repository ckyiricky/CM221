# CM221
## Generate random data
1. Compile generate.cpp
2. run './generate X Y'

X is the number of reads and Y is the length of read
It will generate a file named 'data' in the same directory containing 1-million long text (one line) and X reads(X lines) of Y length (total X+1 lines) 

## Run different methods
1. Compile suffix_array_matching.cpp, better_bw_matching.cpp, suffix_array_mismatch for 3 methods
2. For the first two methods, run './methods inputfile(optional)' which will read text and reads from inputfile (default 'data')
3. For the third method, run './method inputfile(optional) d(optional)' which will read text and reads from inputfile (default 'data') and allow max d mismatches (default 0).
4. Each method will output result of every read to stdout

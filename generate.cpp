/*
 * generate.cpp
 * Copyright (C) 2020 rick <ckyiricky@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <ctime>
#include <random>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define LENGTH 1000000
#define READS 100000
#define PATTERN_LENGTH 100

int main(int argc, char** argv)
{
    vector<char> chars = {'A', 'C', 'T', 'G'};
    srand(time(0));

    int reads = argc > 1 ? stoi(argv[1]) : READS;
    int patternLength = argc > 2 ? stoi(argv[2]) : PATTERN_LENGTH;

    ofstream fout("data");
    for (int i = 0; i < LENGTH; ++i)
    {
        auto c = chars[random()%4];
        fout << c;
    }
    fout << endl;

    for (int i = 0; i < reads; ++i)
    {
        for (int j = 0; j < patternLength; ++j)
        {
            auto c = chars[random()%4];
            fout << c;
        }
        fout << endl;
    }

    cout << "generate " << reads << " reads of " << patternLength << " length" << endl;

    fout.close();
    return 0;
}

/*
 * suffix_array_matching.cpp
 * Copyright (C) 2020 rick <ckyiricky@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "methods.h"

int main(int argc, char** argv)
{
    string filename = argc > 1 ? argv[1] : "data";
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << filename << " doesn't exist" << endl;
        cout << "please run \"application filename\" or \"application\" which will use default filename \'data\'" << endl;
        return 0;
    }

    string text;
    fin >> text;
    text.push_back('$');
    string pattern;
    vector<string> patterns;
    while (fin >> pattern)
    {
        patterns.push_back(move(pattern));
    }
    //string text = "gactgactaca$";
    //vector<string> patterns = {"act"};
    auto suffixArray = move(createSuffixArray(text));
    patternMatchingWithSuffixArray(suffixArray, text, patterns, 0);
    fin.close();
    return 0;
}

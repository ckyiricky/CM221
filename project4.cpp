#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <fstream>
using namespace std;

//bool isEqual(const string& s1, const string& s2, int pos)
//{
    //for (int i = 0; i < s1.size(); ++i)
    //{
        //if (s1[i] != s2[pos+i]) return false;
    //}
    //return true;
//}

bool isEqual(const string_view& s1, const string_view& s2, int pos)
{
    for (int i = 0; i < s1.size(); ++i)
    {
        if (s1[i] != s2[pos+i]) return false;
    }
    return true;
}

vector<int> createSuffixArray(const string& text)
{
    string_view textView(text);
    map<string_view, int> array;
    for (int i = 1; i <= text.size(); ++i)
    {
        array[textView.substr(text.size()-i)] = text.size() - i;
    }
    vector<int> suffixArray;
    for (auto& [_, value] : array)
        suffixArray.push_back(value);
    return suffixArray;
}

string createMText(const vector<int>& suffixArray, const string& text)
{
    string mText;
    for (auto index : suffixArray)
    {
        auto previousI = index ? index-1 : text.size()-1;
        mText.push_back(text[previousI]);
    }
    return mText;
}

unordered_map<char, int> createFirstOccu(const vector<int>& suffixArray, const string& text)
{
    unordered_map<char, int> firstOccu;
    for (int i = 0; i < suffixArray.size(); ++i)
        if (!firstOccu.count(text[suffixArray[i]])) firstOccu[text[suffixArray[i]]] = i;
    return firstOccu;
}

unordered_map<char, vector<int>> createCount(const string& lastColumn, const string& text)
{
    unordered_map<char, vector<int>> count;
    for (auto c : text)
        count[c] = {0};
    for (auto c : lastColumn)
    {
        for (auto& [key, value] : count)
            if (key == c)
                count[key].push_back(value.back() + 1);
            else
                count[key].push_back(value.back());
    }
    return count;
}

pair<int, int> patternMatchSuffix(const vector<int>& suffixArray, const string& text, const string_view& pattern)
{
    int minIndex = 0;
    int maxIndex = text.size() - 1;
    int midIndex;
    while (minIndex <= maxIndex)
    {
        midIndex = minIndex + (maxIndex - minIndex)/2;
        if (pattern > text.substr(suffixArray[midIndex])) minIndex = midIndex + 1;
        else maxIndex = midIndex - 1;
    }
    int first = minIndex;
    if (!isEqual(pattern, text, suffixArray[minIndex]))
    {
        //cout << pattern << " doesn't exist in text" << endl;
        return {-1, -1};
    }
    maxIndex = text.size() - 1;
    while (minIndex <= maxIndex)
    {
        midIndex = minIndex + (maxIndex - minIndex)/2;
        if (pattern < text.substr(suffixArray[midIndex], pattern.size())) maxIndex = midIndex - 1;
        else minIndex = midIndex + 1;
    }
    return {first, maxIndex};
}

//void patternMatchingWithSuffixArray(const vector<int>& suffixArray, const string& text, const vector<string>& patterns)
//{

    //cout << "-------------SuffixArray match algorihtm---------------------" << endl;
    //if (patterns.empty()) return;
    ////auto suffixArray = move(createSuffixArray(text));

    //for (auto& pattern : patterns)
    //{
        //int minIndex = 0;
        //int maxIndex = text.size() - 1;
        //int midIndex;
        //while (minIndex <= maxIndex)
        //{
            //midIndex = minIndex + (maxIndex - minIndex)/2;
            //if (pattern > text.substr(suffixArray[midIndex])) minIndex = midIndex + 1;
            //else maxIndex = midIndex - 1;
        //}
        //int first = minIndex;
        //if (isEqual(pattern, text, suffixArray[minIndex]))
            //cout << pattern << " first: " << minIndex;
        //else
        //{
            //cout << pattern << " doesn't exist in text" << endl;
            //continue;
        //}
        //maxIndex = text.size() - 1;
        //while (minIndex <= maxIndex)
        //{
            //midIndex = minIndex + (maxIndex - minIndex)/2;
            //if (pattern < text.substr(suffixArray[midIndex], pattern.size())) maxIndex = midIndex - 1;
            //else minIndex = midIndex + 1;
        //}
        //cout << " last: " << maxIndex << " total: " << maxIndex - first + 1 << endl;
    //}
//}

void betterBWMatching(const vector<int>& suffixArray, const string& text, const vector<string>& patterns)
{
    cout << "-------------BetterBWMatch algorihtm---------------------" << endl;
    if (patterns.empty()) return;
    //auto suffixArray = move(createSuffixArray(text));
    auto lastColumn = move(createMText(suffixArray, text));
    auto firstOccu = move(createFirstOccu(suffixArray, text));
    auto count = move(createCount(lastColumn, text));
    //cout << "lastcol: " << lastColumn << endl;
    //for (auto& [key, value] : firstOccu)
        //cout << "key: " << key << " value: " << value <<endl;

    //for (auto& [key, value] : count)
    //{
        //cout << key;
        //for (auto c : value) cout << " " << c;
        //cout << endl;
    //}
    for (auto& pattern : patterns)
    {
        if (pattern.empty()) continue;
        int top = 0;
        int bottom = lastColumn.size() - 1;
        int index = pattern.size() - 1;
        while (top <= bottom)
        {
            auto sym = pattern[index];
            if (!firstOccu.count(sym)) break;
            top = firstOccu.at(sym) + count.at(sym)[top];
            bottom = firstOccu.at(sym) + count.at(sym)[bottom+1] - 1;
            --index;
        }
        if (top > bottom || index != -1) cout << pattern << " doesn't exist in text" << endl;
        else cout << pattern << "  first: " << top << " last: " << bottom << " total: " << bottom - top + 1 << endl;
    }
}

void seedExtension(const vector<int>& suffixArray, const string& text, const string& pattern, const pair<int, int>& matchPoints, int start, int end, int mismatch, set<int>& matched)
{
    for (int i = matchPoints.first; i <= matchPoints.second; ++i)
    {
        int textStart = suffixArray[i];
        int miss = 0;
        int pLeft = start, tLeft = suffixArray[i];
        int pRight = end, tRight = suffixArray[i] + end - start;
        if (pLeft > tLeft) continue;
        if (pattern.size() - pRight > text.size() - tRight) continue;
        while (pLeft >= 0 && tLeft >= 0)
        {
            if (pattern[pLeft] != text[tLeft]) ++miss;
            if (miss > mismatch) break;
            --pLeft;
            --tLeft;
        }
        if (miss > mismatch) continue;
        while (pRight < pattern.size() && tRight < text.size())
        {
            if (pattern[pRight] != text[tRight]) ++miss;
            if (miss > mismatch) break;
            ++pRight;
            ++tRight;
        }
        if (miss > mismatch) continue;
        matched.insert(tLeft+1);
    }
}

void patternMatchingWithSuffixArray(const vector<int>& suffixArray, const string& text, const vector<string>& patterns, int mismatch)
{
    cout << "-------------SuffixArray match algorihtm with " << mismatch << " mismatch---------------------" << endl;
    if (patterns.empty()) return;

    for (const auto& pattern : patterns)
    {
        int interval = pattern.size()/(mismatch+1);
        set<int> matched;
        string_view pView(pattern);
        for (int i = 0; i < mismatch; ++i)
        {
            auto result = move(patternMatchSuffix(suffixArray, text, pView.substr(i*interval, interval)));
            if (result.first == -1) continue;
            else
            {
                //seed extension
                seedExtension(suffixArray, text, pattern, result, i*interval, i*interval+interval, mismatch, matched);
            }
        }
        auto result = move(patternMatchSuffix(suffixArray, text, pView.substr(mismatch*interval)));
        if (result.first != -1)
        {
            // seed extension
            seedExtension(suffixArray, text, pattern, result, mismatch*interval, pattern.size(), mismatch, matched);
        }
        if (matched.empty()) cout << pattern << " doesn't have a match with " << mismatch << " mismatches" << endl;
        else
        {
            cout << pattern << " has " << matched.size() << " match(es) with " << mismatch << " mismatches. Starting point in text:" << endl;
            for (auto pos : matched) cout << pos << " ";
            cout << endl;
        }
    }
}

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
    betterBWMatching(suffixArray, text, patterns);
    fin.close();
    return 0;
}

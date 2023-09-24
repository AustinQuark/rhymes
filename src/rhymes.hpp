/**
 * @file rhymes.hpp
 * @description 
 * @author AustinQuark
 * @copyright ProjectR
 */

#ifndef RHYMES_HPP
# define RHYMES_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include <limits>

#include "samples.hpp"

using namespace std;

struct wordInfo
{
    string word;
    string wordProcessed = "";
    bool isWord;
    vector<string> phones = vector<string> {};
};

class rhymes
{
    private:
        void dictToMap(const string &dictPath);
        ifstream dict;
        bool dictCheck();
        map<string, vector<string>> dictMap;
        vector<string> getPhonesOfWord(const string &word);
        void processText(const string &text);
        vector<wordInfo> wordInfoList;
        int levenshteinDistance(const std::string& s1, const std::string& s2);

    public:
        rhymes();
        rhymes(const string &dictPath);
        ~rhymes();

        void printDict();
        void getPhonesOfText(const string &text);
        vector<wordInfo> getWordInfoList();
};

rhymes::rhymes()
{
    cout << "No dictionary path provided." << endl;
}

rhymes::rhymes(const string &dictPath)
{
    dictToMap(dictPath);
    //cout << "Dictionary loaded." << endl;
}

rhymes::~rhymes()
{
    dictMap.clear();
}

void rhymes::dictToMap(const string &dictPath)
{
    dict.open(dictPath);
    string line, word;

    if (!dict.is_open())
    {
        cout << "Error opening dictionary file." << endl;
        return ;
    }
    

    while (getline(dict, line))
    {
        if (!isalpha(line.c_str()[0]))
            continue;

        istringstream iss(line);
        iss >> word;

        vector<string> phones;
        string phone;

        while (iss >> phone) {
            phones.push_back(phone);
        }

        dictMap.insert(pair<string, vector<string>>(word, phones));
        line.clear();
    }
    dict.close();
}

void rhymes::printDict()
{
    if (!dictCheck())
        return ;

    for (auto it = dictMap.begin(); it != dictMap.end(); it++)
    {
        cout << it->first << " : ";

        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            cout << *it2 << " ";
        }
        cout << endl;
    }
}

bool rhymes::dictCheck()
{
    if (!dictMap.size())
    {
        cout << "Dictionary is empty." << endl;
        return false;
    }
    return true;
}

vector<wordInfo> rhymes::getWordInfoList()
{
    return wordInfoList;
}

int rhymes::levenshteinDistance(const std::string& s1, const std::string& s2) {
    int m = s1.length();
    int n = s2.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[m][n];
}

vector<string> rhymes::getPhonesOfWord(const string &word)
{
    if (dictMap.find(word) == dictMap.end())
    {
        int minDist = numeric_limits<int>::max();
        map<string, vector<string>>::iterator closest;

        for (auto it = dictMap.begin(); it != dictMap.end(); it++)
        {
            if (levenshteinDistance(word, it->first) <= minDist)
            {
                minDist = levenshteinDistance(word, it->first);
                closest = it;
            }
        }
        cout << "Word " << word << " not found. Did you mean " << closest->first << "?" << endl;
        return closest->second;
    }
    vector<string> phones = dictMap[word];
    return phones;
}

void rhymes::processText(const string &text)
{
    size_t i = 0, j;

    while (text[i])
    {
        wordInfo newWordInfo;
        j = i;
        if (isalpha(text[i]))
        {
            while (isalpha(text[j]))
                j++;
            newWordInfo.word = text.substr(i, j - i);
            newWordInfo.wordProcessed = newWordInfo.word;
            transform(newWordInfo.wordProcessed.begin(), newWordInfo.wordProcessed.end(), newWordInfo.wordProcessed.begin(), ::toupper);
            newWordInfo.isWord = true;
        }
        else
        {
            while (!isalpha(text[j]))
                j++;
            newWordInfo.word = text.substr(i, j - i);
            newWordInfo.isWord = false;
        }
        wordInfoList.push_back(newWordInfo);
        i += j - i;
    }
}

void rhymes::getPhonesOfText(const string &text)
{
    processText(text);

    for (auto it = wordInfoList.begin(); it != wordInfoList.end(); it++)
        if (it->isWord)
            it->phones = getPhonesOfWord(it->wordProcessed);
}


#endif

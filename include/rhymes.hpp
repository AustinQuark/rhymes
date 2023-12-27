/**
 * @file rhymes.hpp
 * @description 
 * @author AustinQuark
 * @copyright ProjectR
 */

#ifndef RHYMES_HPP
#define RHYMES_HPP

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
#include <stdexcept>
#include <locale>
#include <codecvt>


using namespace std;

struct wordInfo
{
    wstring word;
    wstring wordProcessed = L"";
    bool isWord;
    size_t numPhones = 0;
    vector<wstring> phones = vector<wstring> {};
};

class rhymes
{
    private:
        vector<wordInfo> wordInfoList;
        map<wstring, vector<wstring>> dictMap;
        vector<vector<wstring>> gridPhones;

        void processText(const wstring &text);
        vector<wstring> getPhonesOfWord(const wstring &word);
        void processPhonesOfText();
        int levenshteinDistance(const std::wstring& s1, const std::wstring& s2);
        vector<vector<wstring>> getGridPhones();

    public:
        rhymes();
        rhymes(const string &dictPath);
        ~rhymes();
        
        void dictToMap(const string &dictPath);
        void printDict();
        vector<wordInfo> getWordInfoList();
        void getRhymes(const wstring &text);

};

#endif
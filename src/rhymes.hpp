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

#include "samples.hpp"

using namespace std;

class rhymes
{
    private:
        void dictToMap(const string &dictPath);
        ifstream dict;
        bool dictCheck();
        map<string, vector<string>> dictMap;
        vector<string> getPhonesOfWord(const string &word);


    public:
        rhymes();
        rhymes(const string &dictPath);
        ~rhymes();

        void printDict();
        vector<vector<string>> getPhonesOfText(const string &text);
};

rhymes::rhymes()
{
    cout << "No dictionary path provided." << endl;
}

rhymes::rhymes(const string &dictPath)
{
    dictToMap(dictPath);
    cout << "Dictionary loaded." << endl;
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
        return false;
    return true;
}

vector<string> rhymes::getPhonesOfWord(const string &word)
{
    string copy(word);
    transform(copy.begin(), copy.end(), copy.begin(), ::toupper);

    if (dictMap.find(copy) == dictMap.end())
    {
        cout << "Word '" << word << "' not found" << endl;
        return vector<string> {"NOT FOUND"};
    }

    vector<string> phones = dictMap[copy];
    return phones;
}

/*
vector<vector<string>> rhymes::getPhonesOfText(const string &text)
{
    vector<vector<string>> phonesText;
    vector<string> phonesWord;

    istringstream iss(text);
    stringbuf *p_buf = iss.rdbuf();

    string word;

    while (p_buf->sgetc())
    {
        cout << p_buf << endl;

        if (p_buf->sgetc() == '\r')
        {
            phonesText.push_back(vector<string> {"\r"});
        }
        else
        {
            iss >> word;
            cout << word << endl;
            phonesWord = getPhonesOfWord(word);
            phonesText.push_back(phonesWord);
        }

        word.clear();
    }

    return phonesText;
}
*/

vector<vector<string>> rhymes::getPhonesOfText(const string &text)
{
    vector<vector<string>> phonesText;
    vector<string> phonesWord;

    istringstream iss(text);

    string word;

    while (iss >> word)
    {
        phonesWord = getPhonesOfWord(word);
        phonesText.push_back(phonesWord);
    }
    word.clear();//SEMBLERAIT QUE SEUL LE PREMIER MOT SOIT PRIS EN COMPTE

    return phonesText;
}


#endif

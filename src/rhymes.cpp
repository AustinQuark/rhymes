/**
 * @file rhymes.cpp
 * @description 
 * @author AustinQuark
 * @copyright ProjectR
 */

#include "rhymes.hpp"

rhymes::rhymes()
{
    throw invalid_argument("Dictionary path not specified.");
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
    wifstream dict;

    setlocale(LC_ALL,"");
    dict.open(dictPath);
    dict.imbue(locale(locale(), new codecvt_utf8<wchar_t,0x10ffff, consume_header>));
    
    wstring line, word;

    if (!dict.is_open())
        throw runtime_error("Error opening dictionary file.");

    while (getline(dict, line))
    {
        if (!iswalpha(line[0]))
            continue;

        wistringstream iss(line);
        iss >> word;
        transform(word.begin(), word.end(), word.begin(), ::towupper);

        vector<wstring> phones;
        wstring phone;

        while (iss >> phone) {
            transform(phone.begin(), phone.end(), phone.begin(), ::towupper);
            phones.push_back(phone);
            if (find(phonesList.begin(), phonesList.end(), phone) == phonesList.end())
                phonesList.push_back(phone);
        }

        dictMap.insert(pair<wstring, vector<wstring>>(word, phones));
        line.clear();
    }
    dict.close();
}

void rhymes::printDict()
{
    for (auto it = dictMap.begin(); it != dictMap.end(); it++)
    {
        wcout << it->first << " : ";

        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            wcout << *it2 << " ";
        }
        wcout << endl;
    }
}

vector<wordInfo> rhymes::getWordInfoList()
{
    return wordInfoList;
}

int rhymes::levenshteinDistance(const std::wstring& s1, const std::wstring& s2) {
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

vector<wstring> rhymes::getPhonesOfWord(const wstring &word)
{
    if (dictMap.find(word) == dictMap.end())
    {
        int minDist = numeric_limits<int>::max();
        map<wstring, vector<wstring>>::iterator closest;

        for (auto it = dictMap.begin(); it != dictMap.end(); it++)
        {
            if (levenshteinDistance(word, it->first) <= minDist)
            {
                minDist = levenshteinDistance(word, it->first);
                closest = it;
            }
        }
        wcout << "Word " << word << " not found. Did you mean " << closest->first << "?" << endl;
        return closest->second;
    }

    vector<wstring> phones = dictMap[word];
    
    return phones;
}

void rhymes::processText(const wstring &text)
{
    size_t i = 0, j;

    while (text[i])
    {
        wordInfo newWordInfo;
        j = i;
        if (iswalpha(text[i]))
        {
            while (iswalpha(text[j]))
                j++;
            if (text[j] == L'\'')
                j++;     
            newWordInfo.word = text.substr(i, j - i);
            newWordInfo.wordProcessed = newWordInfo.word;
            transform(newWordInfo.wordProcessed.begin(), newWordInfo.wordProcessed.end(), newWordInfo.wordProcessed.begin(), ::towupper);
            newWordInfo.isWord = true;
        }
        else
        {
            while (!iswalpha(text[j]))
                j++;
            newWordInfo.word = text.substr(i, j - i);
            newWordInfo.isWord = false;
        }
        wordInfoList.push_back(newWordInfo);
        i += j - i;
    }
}

void rhymes::processPhonesOfText()
{
    for (auto it = wordInfoList.begin(); it != wordInfoList.end(); it++)
        if (it->isWord)
        {
            it->phones = getPhonesOfWord(it->wordProcessed);
            it->numPhones = it->phones.size();
        }      
}

vector<vector<wstring>> rhymes::getGridPhones()
{
    vector<wstring> phones;

    for (auto it = wordInfoList.begin(); it != wordInfoList.end(); it++)
    {
        if (it->isWord)
        {           
            for (auto it2 = it->phones.begin(); it2 != it->phones.end(); it2++)
            {
                wstring phone = *it2;
                phone.erase(std::remove_if(phone.begin(), phone.end(),
                    [](char c) { return std::isdigit(c); }), phone.end());
                phones.push_back(phone);
            }
        }
        else if (it->word.find(L"\n") != wstring::npos)
        {
            gridPhones.push_back(phones);
            phones.clear();
        }
    }

    return gridPhones;
}

void rhymes::getGridScores()
{
    float min = 0, max = 0;


    for (size_t i = 0; i < gridPhones.size(); i++)
    {
        vector<float> scores;
        for (size_t j = 0; j < gridPhones[i].size(); j++)
        {
            float score = 0;
            for (size_t k = 0; k < gridPhones.size(); k++)
            {
                for (size_t l = 0; l < gridPhones[k].size(); l++)
                {
                    if (gridPhones[i][j] == gridPhones[k][l])
                    {
                        score += 1.0 / (abs((int)(i - k)) + abs((int)(j - l)) + 1);
                        if (score < min) min = score;
                        if (score > max) max = score;
                    }

                }
            }
            scores.push_back(score);
        }
        gridScores.push_back(scores);
    }

    for (auto it = gridScores.begin(); it != gridScores.end(); it++)
    {
        for (auto it2 = it->begin(); it2 != it->end(); it2++)
        {
            *it2 = (*it2 - min) / (max - min);
        }
    }
}

void rhymes::printToFile()
{
    wofstream outfile;
    outfile.open("scores.txt");
    for (size_t i = 0; i < gridScores.size(); i++)
    {
        for (size_t j = 0; j < gridScores[i].size(); j++)
        {
            outfile << gridScores[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();
    outfile.open("phones.txt");
    for (size_t i = 0; i < gridPhones.size(); i++)
    {
        for (size_t j = 0; j < gridPhones[i].size(); j++)
        {
            outfile << gridPhones[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();
}

void rhymes::getRhymes(const wstring &text)
{
    processText(text);
    processPhonesOfText();

    vector<vector<wstring>> result = getGridPhones();

    getGridScores();
    

    for (size_t i = 0; i < result.size(); i++)
    {
        for (size_t j = 0; j < result[i].size(); j++)
        {
            wcout << setw(4) << result[i][j] << " ";
        }
        cout << endl;
        for (size_t j = 0; j < result[i].size(); j++)
        {
            wcout << setprecision(2) << fixed << gridScores[i][j] << " ";
        }
        cout << endl << endl;       
    }

    /*
    for (auto it = phonesList.begin(); it != phonesList.end(); it++)
    {
        wcout << *it << endl;
    }
    */
    /*
    for (size_t i = 0; i < wordInfoList.size(); i++)
    {
        if (wordInfoList[i].isWord)
        {
            wcout << wordInfoList[i].word << " : ";
            for (size_t j = 0; j < wordInfoList[i].phones.size(); j++)
            {
                wcout << wordInfoList[i].phones[j] << " ";
            }
            cout << endl;
        }
    }
    */
    
}
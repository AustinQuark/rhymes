/**
 * @file main.cpp
 * @main
 * @author AustinQuark
 * @copyright ProjectR
 */

#include "rhymes.hpp"

void printText(vector<wordInfo> &result)
{
    for (auto it = result.begin(); it != result.end(); it++)
    {
        if (!strcmp(it->word.c_str(), "\n"))
            cout << endl;

        for (auto it2 = it->phones.begin(); it2 != it->phones.end(); it2++)
        {
            cout << *it2 << " ";
        }
    }

}

int main()
{
    rhymes r("./dict/cmudict-0.7b");
    r.getPhonesOfText(THAT);
    vector<wordInfo> result = r.getWordInfoList();

    for (auto it = result.begin(); it != result.end(); it++)
    {
        if (it->isWord)
        {
            if (it->phones == vector<string> {"NOT FOUND"})
                cout << it->word << " ";
            else
                for (auto it2 = it->phones.begin(); it2 != it->phones.end(); it2++)
                    cout << *it2 << " ";
        }
        else
        {
            cout << it->word;
        }
        cout << "|";

    }
    
    return 0;
}
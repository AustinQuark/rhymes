/**
 * @file main.cpp
 * @main
 * @author AustinQuark
 * @copyright ProjectR
 */

#include "rhymes.hpp"

int main()
{
    rhymes r("./dict/cmudict-0.7b");
    vector<vector<string>> phones = r.getPhonesOfText(THAT);

    
    for (auto it = phones.begin(); it != phones.end(); it++)
    {
        for (auto it2 = it->begin(); it2 != it->end(); it2++)
        {
            cout << *it2 << " ";
        }
        if (it + 1 != phones.end())
            cout << "| ";
        else
        cout << endl;
    }

    
    return 0;
}
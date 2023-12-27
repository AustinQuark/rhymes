/**
 * @file main.cpp
 * @main
 * @author AustinQuark
 * @copyright ProjectR
 */

#include "rhymes.hpp"
#include "samples.hpp"

#include <iostream> // Include the necessary header file

void printText(vector<wordInfo> &result)
{
    for (auto it = result.begin(); it != result.end(); it++)
    {
        if (!wcscmp(it->word.c_str(), L"\n")) // Convert string literal to wide string literal
            wcout << endl; // Use wcout for wide characters

        for (auto it2 = it->phones.begin(); it2 != it->phones.end(); it2++)
        {
            wcout << *it2 << " "; // Use wcout for wide characters
        }
    }
}

int main()
{
    rhymes r("include/cmudict_fr.txt"); // Use wide string literal for constructor argument
    //r.dictToMap("include/acronym-0.7b"); // Use wide string literal for function argument
    //r.dictToMap("include/cmudict-0.7b"); // Use wide string literal for function argument
    r.getRhymes(LESRAM); // Use wide string literal for function argument
    //r.printDict();
    return 0;
}
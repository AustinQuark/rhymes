#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <regex>
#include <fstream>
#include <iomanip>
#include <functional>
#include "espeak-ng/speak_lib.h"

using namespace std;

struct    s_cell_rhyme{
    string           word;
    bool             to_phonetize;
    vector<string>   phonemes;
    u_int8_t         line_index;
    vector<u_int8_t> phonemes_index;
} typedef t_cell_rhyme;

struct    s_vowels{
    u_int32_t             code;
    vector<string>        symbols;
    vector<bool>          not_found;
    vector<array<int, 3>> pos;
    vector<array<int, 3>> dpos;
} typedef t_vowels;

// Initialize the eSpeak voice synthesis library
int initialize_espeak(const string& voice_name) {
    espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCHRONOUS;
    int buflength = 500;  // Length of sound buffers (in ms)
    const char* path = nullptr;  // Use default path for espeak-ng data
    unsigned int options = 0;  // No special options for initialization

    // Initialize eSpeak
   int sample_rate = espeak_Initialize(output, buflength, path, options);
    if (sample_rate == -1) {
        cerr << "Error initializing eSpeak" << endl;
        return 1;
    }

    // Set the voice
    espeak_ERROR err = espeak_SetVoiceByName(voice_name.c_str());
    if (err != EE_OK) {
        cerr << "Error setting voice espeak_ERROR: " << err << endl;
        return 1;
    }

    return 0;  // Success
}

string text_to_phonemes(const string& text) {
    string phoneme_result;
    const void* text_ptr = text.c_str();
    const char* phonemes;

    while (text_ptr != nullptr) {
        //cout << endl << "Text: " << text << endl;
        phonemes = espeak_TextToPhonemes(&text_ptr, espeakCHARS_UTF8,0);
        if (phonemes != nullptr) {
            phoneme_result += phonemes;
            //cout << phonemes;

        }
    }
    //cout << endl;
    return phoneme_result;
}

vector<string> split_text(const string& text) {
    vector<string> result;
    string word;
    
    for (char c : text) {
        if (ispunct(c) || c == '\n' || isspace(c)) { 
            if (!word.empty()) {
                result.push_back(word);
                word.clear();
            }
            result.push_back(string(1, c));
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        result.push_back(word);
    }

    return result;
}

vector<string> split_phonemes(const string& phonemes, t_vowels* vowels) {
    vector<string> result;
    string phones;
    char * phonemes_ptr = (char*) phonemes.c_str();
    size_t i = -1;
    
    while (phonemes_ptr[++i]) {
        phones += phonemes_ptr[i];
        


        bool delimiterFound = false;
        for (const string& symbol : vowels->symbols) {
            if (phones == symbol) {
                result.push_back(phones);
                phones.clear();
                delimiterFound = true;
                break;
            }
        }=======
        if (!delimiterFound) {
            bool foundLongestDelimiter = false;
            string longestDelimiter;
            
            for (const string& symbol : vowels->symbols) {
                if (symbol.size() > phoneme.size() && symbol.substr(0, phoneme.size()) == phoneme) {
                    longestDelimiter = symbol;
                    foundLongestDelimiter = true;
                }
            }
            if (foundLongestDelimiter) {
                continue;
            }
        }
        if (!phoneme.empty() && delimiterFound) {
            result.push_back(phoneme);
            phoneme.clear();
        } else if (c == '\'') {
            if (!phoneme.empty() && phoneme.back() != '\'') {
                phoneme += c;
            }
        }
    }
    
    if (!phoneme.empty()) {
        result.push_back(phoneme);
    }
    
    return result;
}

void process_text_and_phonemes(const string& text, t_vowels* vowels) {

    vector<vector<t_cell_rhyme>>        cells;
    vector<t_cell_rhyme>                line_cell;
    u_int8_t                            line_index = 0;

    vector<string> words = split_text(text);

    for (const string& word : words) {
        
        if (!ispunct(word[0]) && word[0] != '\n' && !isspace(word[0]) ) {
            string output = text_to_phonemes(word);
            vector<string> phonemes = split_phonemes(output, vowels);

            for (const string& phoneme : phonemes) {
                cout << phoneme << "\n";
            }

            t_cell_rhyme cell = {word, true, phonemes, line_index};
            line_cell.push_back(cell);
        } else if (word[0] == '\n'){
            cells.push_back(line_cell);
            line_cell.clear();
            line_index++;
        }
        else{
            t_cell_rhyme cell = {word, false, {""}, line_index};
            line_cell.push_back(cell);
        }
    }


}

t_vowels* load_vowels(const string& vowels_file) {
    ifstream file(vowels_file);
    t_vowels *vowels = new t_vowels;

    if (!file.is_open()) {
        cerr << "Error opening file: " << vowels_file << endl;
        throw runtime_error("Error opening file");
    }

    regex pattern("^\\s*(\\S{1,2})\\s*([0-9]*)\\s*([0-9]*)\\s*([0-9]*)\\s*([0-9]*)\\s*([0-9]*)\\s*([0-9]*)\\s*([0-9]*)\\s*$");
    smatch match;
    string line;
    while (getline(file, line) && !line.empty()) {
        if (regex_search(line, match, pattern)) {
            if (match.size() != 9) {
                throw runtime_error("Invalid number of matches");
            }
            vowels->symbols.push_back(match[1]);
            vowels->code = hash<string>{}(match[1].str() + to_string(static_cast<int>(vowels->not_found.size())));
            vowels->not_found.push_back(static_cast<bool>(stoi(match[2])));
            array<int, 3> pos = {std::stoi(match[3]), std::stoi(match[4]), std::stoi(match[5])};    
            array<int, 3> dpos = {std::stoi(match[6]), std::stoi(match[7]), std::stoi(match[8])};
            vowels->dpos.push_back(dpos);
            vowels->pos.push_back(pos);
        } else {
            throw runtime_error("Regex search failed");
        }
    }

    if (vowels->symbols.empty()) {
        throw runtime_error("No vowels found in file");
    }
    return vowels;
}

int main() {
    string voice_name = "fr"; 

    if (initialize_espeak(voice_name) != 0) {
        return 1; 
    }

    t_vowels * vowels = load_vowels("./espeak-ng/docs/phonemes/vowelcharts/fr");
    cerr << "Vowels loaded: " << vowels->symbols.size() << endl;
    for (size_t i = 0; i < vowels->symbols.size(); i++) {
        cout << setw(20) << "Symbol: " << vowels->symbols[i];
        cout << setw(20) << "Atrbt: " << vowels->not_found[i];
        cout << setw(20) << "X/Y/Z: " << vowels->pos[i][0] << ", " << vowels->pos[i][1] << ", " << vowels->pos[i][2];
        cout << setw(20) << "dX/dY/dZ: " << vowels->dpos[i][0] << ", " << vowels->dpos[i][1] << ", " << vowels->dpos[i][2];
        cout << endl;
    }

    string sentence = "Ce qui est créé par l’esprit est plus vivant \nque la matière.";

    process_text_and_phonemes(sentence, vowels);

    espeak_Terminate();
    free (vowels);
    return 0;
}

/**
 * @file main.c
 * @description 
 * @author AustinQuark
 * @copyright ProjectR
 */

#include <espeak-ng/speak_lib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
 
espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;
espeak_ERROR err = EE_OK;
char *path = NULL;
void* user_data;
unsigned int *identifier;
int phonememode_ipa_separator = 0x01 | (1 << 7); // IPA phoneme names with '-' as separator
char *delimiters = " \t\n\r.,;:!?\"'()[]{}<>-";

int main(int argc, char* argv[]) {
    char voicename[] = {"fr"}; // Set voice by its name
    char *text = malloc(sizeof(char) * 100); // Replace 100 with the desired size of the text buffer
    memset(text, 0, sizeof(char) * 100);
    strcpy(text, "Oui bonjour, allo, j'fais des tests\nCa marche bien ?");
    const void *textptr = text;
    //char text[] = {"Oui bonjour, allo"};
    int buflength = 500, options = 0;
    unsigned int position = 0, position_type = 0, end_position = 0, flags = espeakCHARS_AUTO;
    espeak_Initialize(output, buflength, path, options);

    err = espeak_SetVoiceByName(voicename);
    if (err != EE_OK) {
        printf("Error setting voice espeak_ERROR : %d\n", err);
        return 1;
    }
    //ESPEAK_API const char *espeak_TextToPhonemes(const void **textptr, int textmode, int phonememode);
    const char **grid_phonemes = malloc(sizeof(char*) * 100); // Replace 100 with the desired size of the phoneme buffer
    memset(grid_phonemes, 0, sizeof(char*) * 100);
    const char ***tmp = malloc(sizeof(char*) * 100);
    memset(tmp, 0, sizeof(char*) * 100);
    uint8_t i = 0;
    while (textptr != NULL) {
        const char *phonemes = espeak_TextToPhonemes(&textptr, espeakCHARS_AUTO, phonememode_ipa_separator);
        printf("Phonemes: %s\n", phonemes);
        grid_phonemes[i] = malloc(sizeof(char) * strlen(phonemes) + 1);
        memset(grid_phonemes[i], 0, sizeof(char) * strlen(phonemes) + 1);
        strcpy(grid_phonemes[i], phonemes);
        printf("A\n");
        uint8_t j = 0;
        char *str_cpy = strdup(phonemes);
        tmp[i][j++] = strtok(str_cpy, delimiters);
        printf("B\n");
        while (tmp[i][j] = strtok(NULL, delimiters))
        {
            printf("C\n");
           j++;
        }
        printf("B\n");
        free(str_cpy);
        printf("C\n");
        i++;
    }
   

    i = -1;
    while (grid_phonemes[++i] != NULL) {
        write(1, grid_phonemes[i], strlen(grid_phonemes[i]));
    }
    write(1, "\n", 1);

    i = -1;
    uint8_t j = 0;
    while (tmp[++i] != NULL) {
        while (*tmp[i] != NULL) {
            printf("__%s__", *tmp[i]);
            j++;
        }
        printf("\n");
    }
    
    


    return 0;
}

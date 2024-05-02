#include "morse.h"
#include <cstring> // For strcmp
#include <cctype> // For toupper

// Default Morse code mapping
const MorseMapping default_mapping[] = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
    {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
    {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
    {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."},
    {'0', "-----"}
};

// Custom Morse code mapping
const MorseMapping *custom_mapping = nullptr;
std::size_t custom_mapping_size = 0;

// Function to set custom mapping
void set_custom_mapping(const MorseMapping *mapping, std::size_t mapping_size) {
    custom_mapping = mapping;
    custom_mapping_size = mapping_size;
}

// Function to convert text to Morse code
void text_to_morse(const char *text, char *morse) {
    const MorseMapping *mapping = custom_mapping ? custom_mapping : default_mapping;
    std::size_t mapping_size = custom_mapping ? custom_mapping_size : sizeof(default_mapping) / sizeof(default_mapping[0]);

    morse[0] = '\0'; // Clear morse string

    for (std::size_t i = 0; i < strlen(text); ++i) {
        char character = toupper(text[i]);

        // Check if the character is valid
        if ((character < 'A' || character > 'Z') && (character < '0' || character > '9')) {
            strcat(morse, " "); // Space between words
            continue;
        }

        // Find the Morse code for the character
        int found = 0;
        for (std::size_t j = 0; j < mapping_size; ++j) {
            if (mapping[j].character == character) {
                strcat(morse, mapping[j].morse);
                strcat(morse, "\n"); // Change line between letters
                found = 1;
                break;
            }
        }
        if (!found) {
            strcat(morse, "? "); // Unknown character
        }
    }
}

// Function to convert Morse code to text
void morse_to_text(const char *morse, char *text) {
    const MorseMapping *mapping = custom_mapping ? custom_mapping : default_mapping;
    std::size_t mapping_size = custom_mapping ? custom_mapping_size : sizeof(default_mapping) / sizeof(default_mapping[0]);

    text[0] = '\0'; // Clear text string

    const char *delimiter = " ";
    char *token = strtok((char *)morse, delimiter);
    while (token != NULL) {
        int found = 0;
        for (std::size_t j = 0; j < mapping_size; ++j) {
            if (strcmp(token, mapping[j].morse) == 0) {
                char temp[2] = {mapping[j].character, '\0'};
                strcat(text, temp);
                found = 1;
                break;
            }
        }
        if (!found) {
            strcat(text, "?");
        }
        token = strtok(NULL, delimiter);
    }
}

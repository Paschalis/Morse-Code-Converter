#include "morse.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Default Morse code mappings
const MorseMapping default_mapping[] = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."},
    {'D', "-.."}, {'E', "."}, {'F', "..-."},
    {'G', "--."}, {'H', "...."}, {'I', ".."},
    {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
    {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},
    {'S', "..."}, {'T', "-"}, {'U', "..-"},
    {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
    {'Y', "-.--"}, {'Z', "--.."},
    {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."},
    {'7', "--..."}, {'8', "---.."}, {'9', "----."},
    {'0', "-----"}, {' ', " "}
};

// Global variable to store custom mapping array
const MorseMapping *custom_mapping = NULL;
size_t custom_mapping_size = 0;

// Function to convert text to Morse code
void text_to_morse(const char *text, char *morse) {
    const MorseMapping *mapping = custom_mapping ? custom_mapping : default_mapping;
    size_t mapping_size = custom_mapping ? custom_mapping_size : sizeof(default_mapping) / sizeof(default_mapping[0]);

    size_t len = strlen(text);
    morse[0] = '\0'; // Clear morse string

    for (size_t i = 0; i < len; ++i) {
        char c = toupper(text[i]);
        if (c == '\0') {
            break; // Stop processing if null terminator encountered
        }
        // Find character in mapping
        int found = 0;
        for (size_t j = 0; j < mapping_size; ++j) {
            if (mapping[j].character == c) {
                strcat(morse, mapping[j].morse);
                strcat(morse, "\n");
                found = 1;
                break;
            }
        }
        if (!found) {
            // Character not found in mapping
            fprintf(stderr, "Error: Character '%c' not supported\n", c);
            morse[0] = '\0'; // Clear morse string
            break;
        }
    }
}


// Function to convert Morse code to text
void morse_to_text(const char *morse, char *text) {
    const MorseMapping *mapping = custom_mapping ? custom_mapping : default_mapping;
    size_t mapping_size = custom_mapping ? custom_mapping_size : sizeof(default_mapping) / sizeof(default_mapping[0]);

    text[0] = '\0'; // Clear text string

    const char *delimiter = " ";
    char *token = strtok((char *)morse, delimiter);
    while (token != NULL) {
        // Check if the token is a valid Morse code
        int is_valid_morse = 0;
        for (size_t i = 0; i < strlen(token); i++) {
            if (token[i] != '.' && token[i] != '-') {
                is_valid_morse = 0;
                break;
            }
            is_valid_morse = 1;
        }
        if (!is_valid_morse) {
            fprintf(stderr, "Error: Invalid Morse code '%s'\n", token);
            text[0] = '\0'; // Clear text string
            return;
        }

        int found = 0;
        for (size_t j = 0; j < mapping_size; ++j) {
            if (strcmp(token, mapping[j].morse) == 0) {
                // Append the corresponding character to the text string
                char temp[2] = {mapping[j].character, '\0'};
                strcat(text, temp);
                found = 1;
                break;
            }
        }
        if (!found) {
            // If Morse code not found in mapping, append '?' to text string
            strcat(text, "?");
        }
        //strcat(text, " "); // Add space between letters
        token = strtok(NULL, delimiter);
    }

    // Remove trailing space
    size_t text_len = strlen(text);
    if (text_len > 0 && text[text_len - 1] == ' ') {
        text[text_len - 1] = '\0';
    }
}


// Function to set custom mapping
void set_custom_mapping(const MorseMapping *mapping, size_t mapping_size) {
    custom_mapping = mapping;
    custom_mapping_size = mapping_size;
}


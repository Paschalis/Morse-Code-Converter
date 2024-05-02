#ifndef MORSE_H
#define MORSE_H

#include <cstddef> // For size_t
#include <string> // For std::string

#define MAX_MORSE_LEN 10

// Struct for mapping between characters and Morse code
struct MorseMapping {
    char character;
    char morse[MAX_MORSE_LEN + 1]; // +1 for null terminator
};

// Declare custom_mapping as extern
extern const MorseMapping *custom_mapping;
extern std::size_t custom_mapping_size;

// Function prototypes
void text_to_morse(const char *text, char *morse);
void morse_to_text(const char *morse, char *text);
void set_custom_mapping(const MorseMapping *mapping, std::size_t mapping_size);

#endif /* MORSE_H */

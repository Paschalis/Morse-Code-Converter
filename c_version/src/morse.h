#ifndef MORSE_H
#define MORSE_H

#include <stddef.h> // For size_t

#define MAX_MORSE_LEN 10

// Struct for mapping between characters and Morse code
typedef struct {
    char character;
    char morse[MAX_MORSE_LEN + 1]; // +1 for null terminator
} MorseMapping;

// Declare custom_mapping as extern
extern const MorseMapping *custom_mapping;
extern size_t custom_mapping_size;

// Function prototypes
void text_to_morse(const char *text, char *morse);
void morse_to_text(const char *morse, char *text);
void set_custom_mapping(const MorseMapping *mapping, size_t mapping_size);

#endif /* MORSE_H */

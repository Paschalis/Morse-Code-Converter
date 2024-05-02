#include "morse.h"
#include "thread_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
    char input_text[BUFFER_SIZE];
    pthread_t text_thread;

    // Normal text to Morse code conversion
    // Initialize IO semaphore with value 1
    if (sem_init(&io_semaphore, 0, 1) != 0) {
        perror("Semaphore initialization failed");
        return 1;
    }

    // Check if command-line argument is provided to indicate Morse to text conversion
    if (argc > 1 && strcmp(argv[1], "-m2t") == 0) {
        // Morse to text conversion
        if (argc < 3) {
            printf("Usage: %s -m2t <Morse code>\n", argv[0]);
            return 1;
        }
        char morse_code[BUFFER_SIZE];
        strncpy(morse_code, argv[2], BUFFER_SIZE);
        morse_code[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
        char text[BUFFER_SIZE];
        morse_to_text(morse_code, text);    
        printf("Text: %s\n", text);
        return 0;
    } 
    // Check if command-line argument is provided to indicate Text to Morse conversion
    else if (argc > 1 && strcmp(argv[1], "-t2m") == 0) {
        // Text to Morse conversion
        if (argc < 3) {
            printf("Usage: %s -t2m <Text>\n", argv[0]);
            return 1;
        }
        char input_text[BUFFER_SIZE];
        strncpy(input_text, argv[2], BUFFER_SIZE);
        input_text[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
        char morse_code[BUFFER_SIZE];
        text_to_morse(input_text, morse_code);
        printf("Morse code: \n%s\n", morse_code);
        return 0;
    }

    // Normal text to Morse code conversion
    // Initialize IO semaphore with value 1
    if (sem_init(&io_semaphore, 0, 1) != 0) {
        perror("Semaphore initialization failed");
        return 1;
    }
    
    if (argc > 1) {
        // Custom mapping array provided via command-line arguments
        MorseMapping *custom_mapping = malloc((argc - 1) * sizeof(MorseMapping));
        if (custom_mapping == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) < 2 || !isalpha(argv[i][0])) {
                fprintf(stderr, "Invalid mapping format: %s\n", argv[i]);
                free(custom_mapping); // Free allocated memory
                return 1;
            }
            int j;
            for (j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] != '.' && argv[i][j] != '-') {
                    fprintf(stderr, "Invalid Morse code symbol: %c\n", argv[i][j]);
                    free(custom_mapping); // Free allocated memory
                    return 1;
                }
            }
            if (j > MAX_MORSE_LEN + 1) {
                fprintf(stderr, "Morse code sequence too long: %s\n", argv[i]);
                free(custom_mapping); // Free allocated memory
                return 1;
            }
            custom_mapping[i - 1].character = argv[i][0];
            strcpy(custom_mapping[i - 1].morse, argv[i] + 1);
        }
        set_custom_mapping(custom_mapping, argc - 1);
    }


    // No command-line argument provided, prompt the user for choice
    printf("Choose an option:\n");
    printf("1. Convert text to Morse code\n");
    printf("2. Convert Morse code to text\n");
    printf("Enter option (1 or 2): ");
    int option;
    scanf("%d", &option);
    getchar(); // Consume newline character

    if (option == 1) {
        // Text to Morse conversion
        printf("Enter text to convert to Morse code: ");
        fgets(input_text, BUFFER_SIZE, stdin);
        input_text[strcspn(input_text, "\n")] = '\0'; // Remove newline character

        // Create text-to-Morse thread
        printf("Creating text-to-Morse thread...\n");
        pthread_create(&text_thread, NULL, text_to_morse_thread, input_text);
        printf("Text-to-Morse thread created.\n");

        // Wait for text-to-Morse thread to complete
        pthread_join(text_thread, NULL);

        // Destroy IO semaphore
        sem_destroy(&io_semaphore);
    } else if (option == 2) {
        // Morse to text conversion
        printf("Enter Morse code to convert to text: ");
        fgets(input_text, BUFFER_SIZE, stdin);
        input_text[strcspn(input_text, "\n")] = '\0'; // Remove newline character

        char text[BUFFER_SIZE];
        morse_to_text(input_text, text);
        printf("Text: %s\n", text);
    } else {
        printf("Invalid option.\n");
    }

    // Wait for text-to-Morse thread to complete
    pthread_join(text_thread, NULL);

    // Destroy IO semaphore
    sem_destroy(&io_semaphore);

    return 0;
}

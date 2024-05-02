#include "morse.h"
#include "thread_utils.h"
#include <iostream>
#include <cstring> // For strncpy
#include <cctype> // For isalpha

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
            std::cout << "Usage: " << argv[0] << " -m2t <Morse code>\n";
            return 1;
        }
        char morse_code[BUFFER_SIZE];
        strncpy(morse_code, argv[2], BUFFER_SIZE);
        morse_code[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
        char text[BUFFER_SIZE];
        morse_to_text(morse_code, text);    
        std::cout << "Text: " << text << std::endl;
        return 0;
    } 
    // Check if command-line argument is provided to indicate Text to Morse conversion
    else if (argc > 1 && strcmp(argv[1], "-t2m") == 0) {
        // Text to Morse conversion
        if (argc < 3) {
            std::cout << "Usage: " << argv[0] << " -t2m <Text>\n";
            return 1;
        }
        char input_text[BUFFER_SIZE];
        strncpy(input_text, argv[2], BUFFER_SIZE);
        input_text[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
        char morse_code[BUFFER_SIZE];
        text_to_morse(input_text, morse_code);
        std::cout << "Morse code: \n" << morse_code << std::endl;
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
        MorseMapping *custom_mapping = new MorseMapping[argc - 1];
        if (custom_mapping == nullptr) {
            std::cerr << "Memory allocation failed\n";
            return 1;
        }
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) < 2 || !isalpha(argv[i][0])) {
                std::cerr << "Invalid mapping format: " << argv[i] << std::endl;
                delete[] custom_mapping; // Free allocated memory
                return 1;
            }
            int j;
            for (j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] != '.' && argv[i][j] != '-') {
                    std::cerr << "Invalid Morse code symbol: " << argv[i][j] << std::endl;
                    delete[] custom_mapping; // Free allocated memory
                    return 1;
                }
            }
            if (j > MAX_MORSE_LEN + 1) {
                std::cerr << "Morse code sequence too long: " << argv[i] << std::endl;
                delete[] custom_mapping; // Free allocated memory
                return 1;
            }
            custom_mapping[i - 1].character = argv[i][0];
            strncpy(custom_mapping[i - 1].morse, argv[i] + 1, MAX_MORSE_LEN);
            custom_mapping[i - 1].morse[MAX_MORSE_LEN] = '\0'; // Ensure null-terminated
        }
        set_custom_mapping(custom_mapping, argc - 1);
    }

    // No command-line argument provided, prompt the user for choice
    std::cout << "Choose an option:\n";
    std::cout << "1. Convert text to Morse code\n";
    std::cout << "2. Convert Morse code to text\n";
    std::cout << "Enter option (1 or 2): ";
    int option;
    std::cin >> option;
    std::cin.ignore(); // Consume newline character

    if (option == 1) {
        // Text to Morse conversion
        std::cout << "Enter text to convert to Morse code: ";
        std::cin.getline(input_text, BUFFER_SIZE);
        
        // Create text-to-Morse thread
        std::cout << "Creating text-to-Morse thread...\n";
        pthread_create(&text_thread, nullptr, text_to_morse_thread, input_text);
        std::cout << "Text-to-Morse thread created.\n";

        // Wait for text-to-Morse thread to complete
        pthread_join(text_thread, nullptr);

        // Destroy IO semaphore
        sem_destroy(&io_semaphore);
    } else if (option == 2) {
        // Morse to text conversion
        std::cout << "Enter Morse code to convert to text: ";
        std::cin.getline(input_text, BUFFER_SIZE);

        char text[BUFFER_SIZE];
        morse_to_text(input_text, text);
        std::cout << "Text: " << text << std::endl;
    } else {
        std::cout << "Invalid option.\n";
    }

    // Wait for text-to-Morse thread to complete
    pthread_join(text_thread, nullptr);

    // Destroy IO semaphore
    sem_destroy(&io_semaphore);

    return 0;
}

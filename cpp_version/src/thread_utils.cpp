#include "thread_utils.h"
#include <iostream>
#include <cstring>

// Define IO semaphore
sem_t io_semaphore;


void *text_to_morse_thread(void *arg) {
    char *text = reinterpret_cast<char *>(arg);
    char morse[100] = ""; // Adjust size as needed
    
    // Enter semaphore
    std::cout << "Thread entering semaphore...\n";
    sem_wait(&io_semaphore);

    text_to_morse(text, morse);

    // Print Morse code
    std::cout << "Morse code: \n" << morse << std::endl;

    // Signal IO semaphore
    std::cout << "Signaling semaphore...\n";
    sem_post(&io_semaphore);

    // Exit semaphore
    std::cout << "Thread exiting semaphore...\n";

    return nullptr;
}

void *morse_to_text_thread(void *arg) {
    char *morse = reinterpret_cast<char *>(arg);
    char text[100] = ""; // Adjust size as needed
    morse_to_text(morse, text);

    // Wait for IO semaphore
    sem_wait(&io_semaphore);

    // Print text
    std::cout << "Text: " << text << std::endl;

    // Signal IO semaphore
    sem_post(&io_semaphore);

    return nullptr;
}

#include "thread_utils.h"
#include "morse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Semaphore for text input/output synchronization
sem_t io_semaphore;

// Function to convert text to Morse code (thread entry point)
void *text_to_morse_thread(void *arg) {
    char *text = (char *)arg;
    char morse[100] = ""; // Adjust size as needed
    
    // Enter semaphore
    printf("Thread entering semaphore...\n");
    sem_wait(&io_semaphore);

    text_to_morse(text, morse);

    // Print Morse code
    printf("Morse code: \n%s\n", morse);

    // Signal IO semaphore
    printf("Signaling semaphore...\n");
    sem_post(&io_semaphore);

    // Exit semaphore
    printf("Thread exiting semaphore...\n");

    return NULL;
}


// Function to convert Morse code to text (thread entry point)
void *morse_to_text_thread(void *arg) {
    char *morse = (char *)arg;
    char text[100] = ""; // Adjust size as needed
    morse_to_text(morse, text);

    // Wait for IO semaphore
    sem_wait(&io_semaphore);

    // Print text
    printf("Text: %s\n", text);

    // Signal IO semaphore
    sem_post(&io_semaphore);

    return NULL;
}

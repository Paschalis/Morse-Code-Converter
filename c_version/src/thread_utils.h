#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include "morse.h"
#include <semaphore.h>

// Declaration of IO semaphore
extern sem_t io_semaphore;

// Function prototypes
void *text_to_morse_thread(void *arg);
void *morse_to_text_thread(void *arg);

#endif /* THREAD_UTILS_H */

/**
 * @file main.c
 * @brief This program creates multiple threads to control GPIO lines using the libgpiod library.
 *
 * Each thread toggles a specific GPIO line on and off in an infinite loop.
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "libgpiod.h"

// Structure to hold thread data
struct thread_data
{
    int thread_num;
    gpiod_line_s* line;
};

// Thread function
void* thread_function(void* arg) {
    struct thread_data thread_data = *((struct thread_data*)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    for(;;)
    {   
        // Set GPIO line to high
        set_gpio(thread_data.line, 1);
        // Set GPIO line to low
        set_gpio(thread_data.line, 0);
    }
}

int main() {
    pthread_t threads[2] = {};
    struct thread_data thread_args[2] = {};
    unsigned int first_chip_line = 77;

    // Create 2 threads
    for (unsigned int i = 0; i < 2; i++) {
        thread_args[i].thread_num = i + 1;
        thread_args[i].line = init_gpio("gpiochip0", first_chip_line + i);

        if (pthread_create(&threads[i], NULL, thread_function, (void*)&thread_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }

        char threadName[20] = {};
        sprintf(threadName, "myThread_%d", i);
        if (pthread_setname_np(threads[i], threadName, 20) != 0)
        {
            perror("cannot set name");
        }
    }

    // Wait for all threads to finish
    for (unsigned int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    printf("All threads have finished.\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;
int shared_data = 0;

/*
This function increments the global 'shared_data' with 
the help of threads. When the threads are fired up together, 
this function will decrement the value of semaphore. If the 
semaphore == 0, then the other threads will wait, untill 
semaphore is incremented again (semaphore > 0). 
*/
void* thread_function(void* arg) {
    sem_wait(&semaphore); // Wait (decrement semaphore)
    
    printf("Thread %d entering critical section.\n", *(int*)arg);
    shared_data++;
    sleep(1); // Simulating work
    printf("Thread %d updated shared_data to %d.\n", *(int*)arg, shared_data);

    sem_post(&semaphore); // Signal (increment semaphore)
    return NULL;
}

int main() {

    pthread_t threads[3];                           // Array of threads that is to be created.
    int thread_ids[3] = {1, 2, 3};                  // Unique IDs for each threads

    sem_init(&semaphore, 0, 1);                     // Initializes semaphores with max of 1 semaphore (can be > 1 as well)

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);     // Creates the threads with unique IDs, 
                                                                                // that will run the thread_function'
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);             // This method waits untill all the threads are done executing
    }

    sem_destroy(&semaphore);                        // Frees up resources used by semaphore
    return 0;
}

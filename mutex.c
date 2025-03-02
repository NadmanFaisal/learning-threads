#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int shared_data = 0;

/*
This function increments the global 'shared_data' with 
the help of threads. When the threads are fired up together, 
this function will lock the mutex, preventing other threads 
from entering. Once the lock has been lifted, other threads
can enter to manipulate the value of the 'shared_data'.
*/
void* thread_function(void* arg) {
    pthread_mutex_lock(&lock);                                                      // Locks the mutex so that only one thread can access
    printf("Thread %d entering critical section.\n", *(int*)arg);                   // Prints the thread ID
    
    shared_data++;                                                                  // Changes the value of shared data
    sleep(1);
    printf("Thread %d updated shared_data to %d.\n", *(int*)arg, shared_data);      // Prints the thread ID and the value of shared data
    
    pthread_mutex_unlock(&lock);                                                    // Unlocks the mutex so that other threads can enter
    return NULL;
}

int main() {
    pthread_t threads[3];                   // Array of threads that is to be created.
    int thread_ids[3] = {1, 2, 3};          // Unique IDs for each thread
    
    pthread_mutex_init(&lock, NULL);        // Initializes mutex

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);         // Creates the threads with their unique IDs,
                                                                                    // which will run the 'thread_function'
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);             // Waits untill all the threads have been executed
    }

    printf("Main function finished.\n");

    pthread_mutex_destroy(&lock);                   // Frees up resources from the mutex
    return 0;
}

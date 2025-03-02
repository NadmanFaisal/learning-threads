/*
The elevator practice problem:- 

You are designing a simulation for a building with two elevators and multiple people who want to use them. The elevators have the following constraints:

    - Each elevator can hold up to 4 people at a time.
    - People can enter only when the elevator is available and not full.
    - Only one person can enter or exit an elevator at a time to prevent collisions.
    - The elevators move independently but cannot exceed the weight limit.

Question: How would you implement a thread-safe elevator system using mutexes and semaphores to ensure that:

    - No more than 4 people enter an elevator at once.
    - People enter one by one (not all at the same time).
    - Multiple elevators operate independently, but each one enforces its own capacity?
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t lift_1_capacity;
sem_t lift_2_capacity;

int lift_1 = 0;
int lift_2 = 0;

pthread_mutex_t walk_lock;

void *getInElevator(void *arg) {
    if(sem_trywait(&lift_1_capacity) == 0) {
        pthread_mutex_lock(&walk_lock);
        lift_1++;
        printf("Person %d entered lift 1, capacity %d/4\n", *(int *) arg, lift_1);
        pthread_mutex_unlock(&walk_lock);
        
        sleep(1);

        pthread_mutex_lock(&walk_lock);
        lift_1--;
        printf("Person %d exited lift 1, capacity %d/4\n", *(int *) arg, lift_1);
        pthread_mutex_unlock(&walk_lock);
        sem_post(&lift_1_capacity);
    } else if(sem_trywait(&lift_2_capacity) == 0) {
        pthread_mutex_lock(&walk_lock);
        lift_2++;
        printf("Person %d entered lift 2, capacity %d/4\n", *(int *) arg, lift_2);
        pthread_mutex_unlock(&walk_lock);
        
        sleep(1);

        pthread_mutex_lock(&walk_lock);
        lift_1--;
        printf("Person %d exited lift 2, capacity %d/4\n", *(int *) arg, lift_2);
        pthread_mutex_unlock(&walk_lock);
        sem_post(&lift_2_capacity);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t people[5];

    int people_id[5] = {1, 2, 3, 4, 5};

    sem_init(&lift_1_capacity, 0, 4);
    sem_init(&lift_2_capacity, 0, 4);
    pthread_mutex_init(&walk_lock, NULL);

    for(int i = 0; i < 5; i++) {
        pthread_create(&people[i], NULL, getInElevator, &people_id[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(people[i], NULL);
    }

    sem_destroy(&lift_1_capacity);
    sem_destroy(&lift_2_capacity);
    pthread_mutex_destroy(&walk_lock);

    return 0;
}
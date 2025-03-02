/*
You are designing a bank ATM system that allows multiple customers to perform transactions concurrently. The ATMs have the following constraints:

    - There are 3 ATMs available.
    - Each ATM can serve only one customer at a time.
    - Customers should wait if all ATMs are occupied.
    - Each customer can perform only one transaction before leaving the ATM.
    - Only one customer can update their account balance at a time to avoid race conditions.

    Question: How would you implement a thread-safe ATM system using mutexes and semaphores to ensure that:

    - No more than 3 customers use ATMs at the same time.
    - Only one customer updates their bank balance at a time.
    - Customers must wait if all ATMs are busy.
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

sem_t use_atm;
pthread_mutex_t lock;

int shared_balance = 1000;

void *useAtm(void *arg) {
    sem_wait(&use_atm);
    
    printf("Customer: %d is using the ATM\n", *(int *) arg);

    pthread_mutex_lock(&lock);
    shared_balance -= 100;
    sleep(1);
    printf("Customer: %d is finished using the ATM, BALANCE: %d\n", *(int *) arg, shared_balance);
    pthread_mutex_unlock(&lock);

    sem_post(&use_atm);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t people[10];
    int people_id[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    sem_init(&use_atm, 0, 3);
    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < 10; i++) {
        pthread_create(&people[i], NULL, useAtm, &people_id[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(people[i], NULL);
    }

    sem_destroy(&use_atm);
    pthread_mutex_destroy(&lock);
}
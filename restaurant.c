#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CUSTOMERS 10

sem_t chef_available;
pthread_mutex_t lock;

int orderQueue = 0;

void *orderFood(void *arg) {
    sem_wait(&chef_available);
    printf("Customer: %d has ordered to chef\n", *(int *)arg);

    pthread_mutex_lock(&lock);
    orderQueue++;
    printf("Chef is cooking for customer: %d, order queue: %d\n", *(int *)arg, orderQueue);
    sleep(1);
    orderQueue--;
    printf("Chef is done cooking for customer: %d, order queue: %d\n", *(int *)arg, orderQueue);
    pthread_mutex_unlock(&lock);

    sem_post(&chef_available);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t customers[MAX_CUSTOMERS];
    int customer_id[MAX_CUSTOMERS];

    sem_init(&chef_available, 0, 3);
    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < MAX_CUSTOMERS; i++) {
        customer_id[i] = i + 1;
        pthread_create(&customers[i], NULL, orderFood, &customer_id[i]);
    }

    for(int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    sem_destroy(&chef_available);
    pthread_mutex_destroy(&lock);
}
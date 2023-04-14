/*
    CS3305 Assignment 5 - Brandon Luu
    assignment-5.c

    Simulate mutex locks in a train station scenario.
*/

// Include whatever we need
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

// Structure definition for a train
typedef struct {
    int destination;
    int passengers;
    int capacity;
    int id;
} train;

// Structure definition for a station
typedef struct {
    int passengers;
    int needed;
    int id;
} station;

// Global variables for station and mutex locks
station station0, station1, station2, station3, station4;
pthread_mutex_t s0Lock, s1Lock, s2Lock, s3Lock, s4Lock, outLock;

// Do the train thread
void* do_train(void* t) {
    train *thetrain = (train*)t;

    // Repeat until we are done
    while (1) {
        // Set the mutex lock to the correct station
        pthread_mutex_t *lock;
        if (thetrain->destination == 0) lock = &s0Lock;
        if (thetrain->destination == 1) lock = &s1Lock;
        if (thetrain->destination == 2) lock = &s2Lock;
        if (thetrain->destination == 3) lock = &s3Lock;
        if (thetrain->destination == 4) lock = &s4Lock;

        // Pick the station
        station *st;
        if (thetrain->destination == 0) st = &station0;
        if (thetrain->destination == 1) st = &station1;
        if (thetrain->destination == 2) st = &station2;
        if (thetrain->destination == 3) st = &station3;
        if (thetrain->destination == 4) st = &station4;

        // Do nothing until we can enter the train's destination station
        while (pthread_mutex_lock(lock) != 0);

        // Stall for print
        while (pthread_mutex_lock(&outLock) != 0);
        printf("Train %d ENTERS Station %d\n", thetrain->id, st->id);

        // Station 0, pick up passengers
        int end = 0;
        if (st->id == 0) {
            printf("\tStation %d has %d passengers to pick up\n", st->id, st->passengers);
            
            // Start picking up passengers
            // If there are passengers to pick up, then...
            if (st->passengers > 0) {
                printf("\tPicking up passengers...\n");
                
                int amount = st->passengers - thetrain->capacity;
                
                // Station will be empty
                if (amount < 0) {
                    // Example: 50 potential passengers / 10 = 5 seconds
                    sleep(st->passengers/10);

                    // Pick up the rest and set the station to 0
                    thetrain->passengers = st->passengers;
                    st->passengers = 0;
                }
                else {
                    // Example: 50 potential passengers / 10 = 5 seconds
                    sleep(thetrain->capacity/10);

                    // Fill train and set station to new amount
                    thetrain->passengers = thetrain->capacity;
                    st->passengers = amount;
                }

                // Goto next station
                thetrain->destination++;
            }
            
            // Otherwise we can end the thread
            else end = 1;

            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", thetrain->id, st->id, thetrain->passengers, thetrain->capacity);
            printf("\tStation %d has %d passengers to pick up\n", st->id, st->passengers);
        }
        // Any other station
        else {
            printf("\tStation %d has %d passengers to drop off\n", st->id, st->needed);

            // Station needs people
            if (thetrain->passengers > 0 && st->needed > 0) {
                printf("\tUnloading passengers...\n");

                int amount = st->needed - thetrain->passengers;
                
                // Station will be full
                if (amount < 0) {
                    // Example: 50 potential passengers / 10 = 5 seconds
                    sleep(st->needed/10);
                    
                    // Drop off the rest and set the station to full
                    // Example: 10 needed - 50 passengers = -40 * -1 = 40 passengers remain
                    thetrain->passengers = -1 * amount;
                    st->needed = 0;
                }
                else {
                    // Example: 50 potential passengers / 10 = 5 seconds
                    sleep(thetrain->capacity/10);

                    // Drop off entire train and set station to new amount
                    thetrain->passengers = 0;
                    st->needed = amount;
                }
            }

            // If there are no passengers in the train left, go backwards, otherwise go forwards
            if (thetrain->passengers <= 0) thetrain->destination--;
            else thetrain->destination++;

            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", thetrain->id, st->id, thetrain->passengers, thetrain->capacity);
            printf("\tStation %d has %d passengers to drop off\n", st->id, st->needed);
        }

        // Done with station, unlock
        pthread_mutex_unlock(lock);

        // End stall to print
        printf("Train %d LEAVES Station %d\n", thetrain->id, st->id);
        pthread_mutex_unlock(&outLock);
        
        if (end) break; // Break if done
    }
}

// Main function, do setup and threading
int main() {
    // Set up/initialize the trains
    train train0;
    train0.destination = 0;
    train0.passengers = 0;
    train0.capacity = 100;
    train0.id = 0;

    train train1;
    train1.destination = 0;
    train1.passengers = 0;
    train1.capacity = 50;
    train1.id = 1;

    // Set up/initialize the 5 stations with station 0 having 500 starting passengers
    station0.id = 0;
    station0.needed = 0;
    station0.passengers = 500;

    station1.id = 1;
    station1.needed = 50;
    station1.passengers = 0;

    station2.id = 2;
    station2.needed = 100;
    station2.passengers = 0;

    station3.id = 3;
    station3.needed = 250;
    station3.passengers = 0;

    station4.id = 4;
    station4.needed = 100;
    station4.passengers = 0;

    // Attempt to initialize each mutex lock for each station and stdout
    if (pthread_mutex_init(&s0Lock, NULL) != 0) {
        printf("Failed to initialize station 0 lock!\n");
        return -1;
    }

    if (pthread_mutex_init(&s1Lock, NULL) != 0) {
        printf("Failed to initialize station 1 lock!\n");
        return -1;
    }

    if (pthread_mutex_init(&s2Lock, NULL) != 0) {
        printf("Failed to initialize station 2 lock!\n");
        return -1;
    }

    if (pthread_mutex_init(&s3Lock, NULL) != 0) {
        printf("Failed to initialize station 3 lock!\n");
        return -1;
    }

    if (pthread_mutex_init(&s4Lock, NULL) != 0) {
        printf("Failed to initialize station 4 lock!\n");
        return -1;
    }

    if (pthread_mutex_init(&outLock, NULL) != 0) {
        printf("Failed to initialize stdout lock!\n");
        return -1;
    }

    // Create our threads and join them
    pthread_t train0_thread, train1_thread;
    pthread_create(&train0_thread, NULL, &do_train, &train0);
    pthread_create(&train1_thread, NULL, &do_train, &train1);
    pthread_join(train0_thread, NULL);
    pthread_join(train1_thread, NULL);

    return 0;
}
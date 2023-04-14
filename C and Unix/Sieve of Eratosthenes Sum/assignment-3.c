/*
    CS3305 Assignment 3
    Brandon Luu

    Compute the sum of all the primes lower than N.
*/

// include our needed headers
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

// global variables for sum and count
// store the sum in an unsigned long to prevent overflow
unsigned long g_sum = 0;
unsigned int g_count = 0;

// find_prime
// finds primes between an interval of low to high, then adds it
// to the global sum (gs) and counter (gc)
void *find_prime(void* args[]) {
    // setup our variables to count
    unsigned int i, flag;
    unsigned long sum = 0;
    unsigned int count = 0;

    // we have a few global variables, we cast them to int
    unsigned int threadnum = ((unsigned int*)args)[0];
    unsigned int low = ((unsigned int*)args)[1];
    unsigned int high = ((unsigned int*)args)[2];
    printf("Thread %u is finding primes from low = %u to high = %u\n", threadnum, low, high);

    // code taken from https://www.programiz.com/c-programming/examples/prime-number-intervals
    // as noted in a section from the assignment page

    // iteration until low is not equal to high
    while (low < high) {
        flag = 0;

        // ignore numbers less than 2
        if (low <= 1) {
            ++low;
            continue;
        }

        // if low is a non-prime number, flag will be 1
        for (i = 2; i <= low / 2; ++i) {

            if (low % i == 0) {
            flag = 1;
            break;
            }
        }

        // if the flag is 0, then we have a prime.
        // add it to our counter pointers
        if (flag == 0) {
            count++;
            sum += low;
        }

        // to check prime for the next number
        // increase low by 1
        ++low;
   }

   // finally, add our sums and counts to the global sum and count
   printf("Sum of thread %u is %u, Count is %u\n", threadnum, sum, count);
   g_sum += sum;
   g_count += count;
}

int main(int argc, char* argv[]) {
    // check argument count, return failure if not 2 arguments
    if (argc != 3) {
        printf("Proper usage is ./assignment-3 <threadCount> <highestInt>\n");
        return -1;
    }

    // get our variables for threadCount and highestInt (N)
    int threadCount = atoi(argv[1]); // number of threads we should use
    int highestInt = atoi(argv[2]); // N
    
    // threads cannot be 0, error if so
    if (threadCount == 0) {
        printf("Thread count cannot be 0\n");
        return -1;
    }

    // split the task evenly, find how much should each thread
    // calculate depending on the count and N
    // floor it so we don't have any funky remainder
    int countPerThread = ceil(highestInt / threadCount);
    int remaining = highestInt % threadCount;

    // do threading for 0 to threadCount threads
    // store threads into array
    pthread_t threads[threadCount];
    for (int i = 0; i != threadCount; i++) {
        unsigned int low = countPerThread * i;
        unsigned int high = countPerThread * (i + 1);

        // account for remainder if it is the very last thread
        if (remaining != 0 && i + 1 == threadCount) high = highestInt;

        // this supposedly does not work as it is on the stack, so
        // it will reference this "twice"?
        // ie. i = 0 runs i = 1 instead
        //long arguments[] = {i, low, high};

        // we will use malloc instead
        unsigned int *arguments = (unsigned int*)malloc(12);
        arguments[0] = i;
        arguments[1] = low;
        arguments[2] = high;

        // create the thread, error if failure
        if (pthread_create(&threads[i], NULL, find_prime, arguments)) {
            printf("Error while creating thread %u\n", i);
        }
    }

    // join up with all our threads, error if failure
    for (int i = 0; i != threadCount; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("Error while joining thread %u\n", i);
        }
    }
    
    // print our grand totals
    printf("\n    GRAND SUM IS %lu, COUNT IS %u\n\n", g_sum, g_count);

    return 0;
}

/*
    CS3305 Assignment 4 - Brandon Luu
    assignment-4.c

    Simulates process scheduling algorithms, more specifically:
    First-Come First-Served
    Shortest Job First
    Round-Robin (with time quantum)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Create a struct for our process
struct Process {
    int id, arrival, burst, wait, turnaround;
};

int main(int argc, char* argv[]) {
    // Setup variables
    char* fileName;
    int mode = -1; // mode = -1 means invalid
    int active = 0, time = 0; // active process and current time
    struct Process* processes = (struct Process*)malloc(0); // processes

    // Argument count check, exit if < 3 args
    if (argc < 3) {
        printf("Proper usage is ./assignment-4 [-f|-s|-r <quantum>] <Input file name>\n");
        return -1;
    }
    
    // Set our mode to the correct argument
    // If round robin selected and missing quantum, exit
    // Otherwise, set file name as the argument

    // Check if Round Robin
    if (strcmp(argv[1], "-r") == 0) {
        // Invalid # of arguments for RR
        if (argc < 4) {
            printf("For round robin, a quantum must be provided: ./assignment-4 -r <quantum>\n");
            return -1;
        }
        // Correct otherwise
        else {
            fileName = argv[3];
            mode = 2;
        }
    }
    else {
        // If additional arguments are supplied, error
        if (argc > 3) {
            printf("Invalid number of arguments for -f or -s\n");
            return -1;
        }
        
        // Set filename
        fileName = argv[2];

        // Set to first come first served or shortest job first
        if (strcmp(argv[1], "-f") == 0) mode = 0;
        else if (strcmp(argv[1], "-s") == 0) mode = 1;
    }

    // If invalid argument provided (set by mode), exit
    if (mode == -1) {
        printf("Invalid mode selected, expected -f|-s|-r <quantum>\n");
        return -1;
    }

    // Open the file
    FILE* file;
    file = fopen(fileName, "r");

    // If file fails to open, exit
    if (file == NULL) {
        printf("Could not open file %s!\n", fileName);
        return -1;
    }

    // Read file contents into array of processes
    int id, arrival;
    int count = 0; // # of processes so far (or the index)

    while (fscanf(file, "P%d,%d\n", &id, &arrival) != 1) {;
        //printf("P%d, %d\n", id, arrival);
        
        // Create process
        struct Process newProc;
        newProc.id = id;
        newProc.arrival = id;
        newProc.burst = arrival;
        newProc.wait = 0;
        newProc.turnaround = 0;

        // realloc() and put in the array
        processes = (struct Process*)realloc(processes, (count + 1) * sizeof(struct Process));
        processes[count] = newProc;
        count++;

        // End of file
        if (feof(file)) break;
    }

    // No processes found in file for whatever reason
    if (count == 0) {
        printf("No processes found in file\n");
        return -1;
    }

    // Depending on our argument, select the mode and run it

    // First Come First Served
    if (mode == 0) {
        printf("-- First Come First Served --\n");
        // Select first process
        active = 0;

        while (1) {
            int finished = 1;

            // Print current time status
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n", time, active, processes[active].burst, processes[active].wait, processes[active].turnaround);

            // Make changes to all processes, increment turnaround and wait if needed
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0 && processes[i].arrival <= 0) {
                    processes[i].turnaround++;

                    if (processes[i].id != active) processes[i].wait++;
                }
                processes[i].arrival--;
            }

            // If there are still processes with burst, we are not done
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0) finished = 0;
            }

            // Update active process burst
            processes[active].burst--;          

            // Active process is done, move onto next
            if (processes[active].burst == 0) active++; 

            // Increment time
            time++;
            
            // No more processes, break
            if (finished || active == count) break;
        }

        // Get totals for our average wait and turn, then print for each process and totals
        float avgwait = 0;
        float avgturn = 0;

        for (int i = 0; i != count; i++) {
            avgwait += processes[i].wait;
            avgturn += processes[i].turnaround;

            printf("P%d\n\tWaiting time:\t\t%d\n\tTurnaround time:\t%d\n", processes[i].id, processes[i].wait, processes[i].turnaround);
        }

        printf("\nAverage waiting time: %.1f\nAverage turnaround time: %.1f\n\n", (float)avgwait/count, (float)avgturn/count);
    }

    // Shortest Job First
    else if (mode == 1) {
        printf("-- Shortest Job First --\n");
        // We will begin with P0 as it is the first to enter
        int shortestTime = processes[0].burst;
        int shortestProc = 0;

        // It is now the active process
        active = shortestProc;

        // Loop until we have no more processes
        while (1) {
            int finished = 1;

            // Print current time status
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n", time, active, processes[active].burst, processes[active].wait, processes[active].turnaround);

            // Make changes to all processes
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0 && processes[i].arrival <= 0) {
                    processes[i].turnaround++;

                    if (processes[i].id != active) processes[i].wait++;
                }
                processes[i].arrival--;
            }

            // If there are still processes with burst, we are not done
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0) finished = 0;
            }

            // Update active process burst
            processes[active].burst--;          

            // We will use preemption to find the next shortest job
            shortestTime = -1;
            shortestProc = -1;

            for (int i = 0; i != count; i++) {
                // Select initial shortest process
                if (shortestTime == -1 && processes[i].burst > 0 && processes[i].arrival <= 0) {
                    shortestTime = processes[i].burst;
                    shortestProc = i;
                }

                // Find shortest
                if (processes[i].burst > 0 && processes[i].burst < shortestTime && processes[i].arrival <= 0) {
                    shortestTime = processes[i].burst;
                    shortestProc = i;
                }
            }

            // Active process is the new shortest one
            active = shortestProc;

            // Increment time
            time++;

            // End if we cannot find another process
            if (finished || shortestProc == -1) break;
        }

        // Get totals for our average wait and turn, then print for each process and totals
        float avgwait = 0;
        float avgturn = 0;

        for (int i = 0; i != count; i++) {
            avgwait += processes[i].wait;
            avgturn += processes[i].turnaround;

            printf("P%d\n\tWaiting time:\t\t%d\n\tTurnaround time:\t%d\n", processes[i].id, processes[i].wait, processes[i].turnaround);
        }

        printf("\nAverage waiting time: %.1f\nAverage turnaround time: %.1f\n", (float)avgwait/count, (float)avgturn/count);
    }

    // Round Robin
    else if (mode == 2) {
        // Get time quantum
        int rrquantum = atoi(argv[2]);
        printf("-- Round Robin with Quantum %d--\n", rrquantum);

        int rrcurrent = rrquantum;

        // Select first process
        active = 0;

        while (1) {
            int finished = 1;

            // Print current time status
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n", time, active, processes[active].burst, processes[active].wait, processes[active].turnaround);

            // Make changes to all processes, increment turnaround and wait if needed
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0 && processes[i].arrival <= 0) {
                    processes[i].turnaround++;

                    if (processes[i].id != active) processes[i].wait++;
                }
                processes[i].arrival--;
            }

            // If there are still processes with burst, we are not done
            for (int i = 0; i != count; i++) {
                if (processes[i].burst > 0) finished = 0;
            }

            // Update active process burst
            processes[active].burst--;

            // Update time and quantum
            time++;
            rrcurrent--;

            if (processes[active].burst <= 0 || rrcurrent <= 0) {
                // Reset time quantum if expired
                rrcurrent = rrquantum;

                // Try to find a process with available burst and loop around if needed.
                // Stop if we have searched all processes and finish
                int currentactive = active + 1;

                while (1) {
                    // If we are at the same active process, we have looped, stop
                    if (currentactive == active) break;
                    // If we are over the process count, set active to start
                    if (currentactive >= count) currentactive = 0;

                    // Burst found, stop
                    if (processes[currentactive].burst > 0) {
                        active = currentactive;
                        break;
                    }

                    currentactive++;
                }
            }
            
            // No more processes, break
            if (finished) break;
        }

        // Get totals for our average wait and turn, then print for each process and totals
        float avgwait = 0;
        float avgturn = 0;

        for (int i = 0; i != count; i++) {
            avgwait += processes[i].wait;
            avgturn += processes[i].turnaround;

            printf("P%d\n\tWaiting time:\t\t%d\n\tTurnaround time:\t%d\n", processes[i].id, processes[i].wait, processes[i].turnaround);
        }

        printf("\nAverage waiting time: %.1f\nAverage turnaround time: %.1f\n\n", (float)avgwait/count, (float)avgturn/count);
    }

    return 0;
}
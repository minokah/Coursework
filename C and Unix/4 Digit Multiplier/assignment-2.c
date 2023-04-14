/*
    CS3305 Assignment 2
    Brandon Luu
    assignment-2.c

    Compute large integer multiplication using fork and pipes.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {
    // partition and storage variables needed for calculation
    int n1, n2;
    int a1, b1, a2, b2;
    int X, Y, Z;

    // temporary variables for pipes, when we read from the pipes we will use these to store the values
    int c1, c2, c3;

    // argument count check, requires 2 integers
    if (argc != 3) {
        printf("Proper usage is ./assignment-2 <4 digit integer> <4 digit integer>\n");
        return -1;
    }
    else {
        // set the variables and partition them based on algorithm
        n1 = atoi(argv[1]);
        a1 = n1 / 100;
        a2 = n1 % 100;

        n2 = atoi(argv[2]);
        b1 = n2 / 100;
        b2 = n2 % 100; 
    }

    // if integers are under or over 4 digits, return an error as we don't support it
    if (n1 < 1000 || n1 > 9999 || n2 < 1000 || n2 > 9999) {
        printf("Proper usage is ./assignment-2 <4 digit integer> <4 digit integer>\n");
        return -1;
    }

    // print the integers and proceed as normal otherwise
    printf("Your integers are %d %d\n", n1, n2);

    // create child to parent pipe
    int parentPipe[2];
    if (pipe(parentPipe) < 0) {
        printf("Failed to establish parent pipe\n");
        return -1;
    }

    // create parent to child pipe
    int childPipe[2];
    if (pipe(childPipe) < 0) {
        printf("Failed to establish child pipe\n");
        return -1;
    }

    // length check, if it is of length 1 then just multiply and print.
    // Disabled because it does not match the supposed output, will return an error instead above
    /*
    if (n1 <= 1) {
        printf("%d * %d = %d\n\n", n1, n2, n1 * n2);
        return 0;
    }
    */

    // fork it!
    pid_t pid = fork();

    // print the parent and child, if child
    if (pid == 0) printf("Parent (PID %d): created child (PID %d)\n", getppid(), getpid());

    // fork failure, exit
    if (pid < 0) {
        printf("Failed to fork\n");
        return -1;
    }
    // child, as pid = 0
    else if (pid == 0) {
        // 2. child multiplies, send back A to parent
        // read pipe
        read(childPipe[0], &c1, 4);
        read(childPipe[0], &c2, 4);
        // print received
        printf("Child (PID %d): Received %d from parent\n", getpid(), c1);
        printf("Child (PID %d): Received %d from parent\n", getpid(), c2);

        // send calculated data back
        c3 = c1 * c2;
        printf("Child (PID %d): Sending %d to parent\n", getpid(), c3);
        write(parentPipe[1], &c3, 4);

        // 4. child multiplies, send back B to parent
        // read pipe
        read(childPipe[0], &c1, 4);
        read(childPipe[0], &c2, 4);
        // print received
        printf("Child (PID %d): Received %d from parent\n", getpid(), c1);
        printf("Child (PID %d): Received %d from parent\n", getpid(), c2);

        // send calculated data back
        c3 = c1 * c2;
        printf("Child (PID %d): Sending %d to parent\n", getpid(), c3);
        write(parentPipe[1], &c3, 4);
        
        // 6. child multiplies, send back C to parent
        // read pipe
        read(childPipe[0], &c1, 4);
        read(childPipe[0], &c2, 4);
        // print received
        printf("Child (PID %d): Received %d from parent\n", getpid(), c1);
        printf("Child (PID %d): Received %d from parent\n", getpid(), c2);

        // send calculated data back
        c3 = c1 * c2;
        printf("Child (PID %d): Sending %d to parent\n", getpid(), c3);
        write(parentPipe[1], &c3, 4);

        // 8. child multiplies, send back D to parent
        // read pipe
        read(childPipe[0], &c1, 4);
        read(childPipe[0], &c2, 4);
        // print received
        printf("Child (PID %d): Received %d from parent\n", getpid(), c1);
        printf("Child (PID %d): Received %d from parent\n", getpid(), c2);

        // send calculated data back
        c3 = c1 * c2;
        printf("Child (PID %d): Sending %d to parent\n", getpid(), c3);
        write(parentPipe[1], &c3, 4);

        // 9. child exits
        return 0;
    }
    // parent, as pid > 0
    else if (pid > 0) {
        printf("\n---- Calculating X  ----\n");
        // 1. parent send a1 and b1 to child
        // print who is sending what
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);
        // write to pipe
        write(childPipe[1], &a1, 4);
        write(childPipe[1], &b1, 4);

        // 2. parent calculates X
        // read pipe
        read(parentPipe[0], &c1, 4);
        // print received
        printf("Parent (PID %d): Received %d from child\n", getpid(), c1);
        // set X
        X = c1 * pow(10, 4);

        printf("\n---- Calculating Y  ----\n");
        // 3. parent send a2 and b1 to child
        // write to pipe
        write(childPipe[1], &a1, 4);
        write(childPipe[1], &b2, 4);
        // print who is sending what
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);

        // 5. parent send a1 and b2 to child
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        write(childPipe[1], &a2, 4);
        write(childPipe[1], &b1, 4);

        // 6. parent calculates Y

        // read pipe
        read(parentPipe[0], &c1, 4);
        read(parentPipe[0], &c2, 4);
        // read received
        printf("Parent (PID %d): Received %d from child\n", getpid(), c1);
        printf("Parent (PID %d): Received %d from child\n", getpid(), c2);
        // calculate Y
        Y = (c1 + c2) * pow(10, 2);

        printf("\n---- Calculating Z  ----\n");
        // 7. parent send a2 and b2 to child
        // print who is sending what
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);
        // write to pipe
        write(childPipe[1], &a2, 4);
        write(childPipe[1], &b2, 4);

        // 8. parent calculates Z
        // read pipe
        read(parentPipe[0], &c1, 4);
        // print received
        printf("Parent (PID %d): Received %d from child\n", getpid(), c1);
        // calculate Z
        Z = c1 * pow(10, 0);

        // calculate the sum, and then print it out
        int sum = X + Y + Z;
        printf("%d * %d = %d + %d + %d = %d\n\n", n1, n2, X, Y, Z, sum);
    }

    // exit success
    return 0;
}
// Assignment 1 - CS3305
// Brandon Luu (251165351)

// This program will attempt to crack a password by brute forcing
// characters. It provides an option to use forking (-f) (or not) and
// to automatically run the hackme program (-p).

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "checkPassword.h"

// solve the password, where n * 3 is the position.
// n will correspond to which process we are referring to (ie. parent, child 1, etc.)
void solve(int n)
{
    // brute force password using triple for loop
    for (int a = 33; a != 127; a++)
    {
        for (int b = 33; b != 127; b++)
        {
            for (int c = 33; c != 127; c++)
            {
                char str[4] = {a, b, c, '\0'};
		// check password
                if (checkPassword(str, n * 3) == 0)
                {
                    printf("%c%c%c\n", str[0], str[1], str[2]);
                    return;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // parse arguments, -f and -p
    int arg_fork = 0;
    int arg_parent = 0;

    for (int i = 0; i != argc; i++)
    {
	// fork argument
        if (strcmp(argv[i], "-f") == 0)
        {
            arg_fork = 1;
            printf("Forking enabled\n");
        }
	// autorun argument
        else if (strcmp(argv[i], "-p") == 0)
        {
            arg_parent = 1;
            printf("Parent will autorun hackme\n");
        }
    }

    // if we are forking...
    if (arg_fork == 1)
    {
        // begin fork
        pid_t firstfork = fork();

        // fork failure, terminate
        if (firstfork < 0) {
            printf("Fork failed! PID: %d", getpid());
            return -1;
        }

        pid_t secondfork;
        printf("PID: %d, PPID: %d\n", getpid(), getppid());

        // second child solve 6-8
        if (firstfork == 0) {
            secondfork = fork();

            // fork failure, terminate
            if (secondfork < 0) {
                printf("Fork failed! PID: %d", getpid());
                return -1;
            }

            printf("PID: %d, PPID: %d\n", getpid(), getppid());
            
            // third child solve 9-12
            if (secondfork == 0) {
                solve(3);
            }
            else {
                solve(2);
                wait(secondfork);
            }
        }
        // parent
        else {
            secondfork = fork();

            // fork failure, terminate
            if (secondfork < 0) {
                printf("Fork failed! PID: %d", getpid());
                return -1;
            }

            printf("PID: %d, PPID: %d\n", getpid(), getppid());
            
            // immediate child of parent, solve 2-5
            if (secondfork == 0) {
                solve(1);
            }
            // parent solve 0-2 and waits
            else {
                solve(0);
                wait(firstfork);
                wait(secondfork);
                if (arg_parent == 1) execl("./hackme", NULL);
            }
        }
    }
    // no -p, so parent will brute force everything
    else
    {
        for (int i = 0; i != 4; i++) solve(i);

        // launch hackme if -p flag
        if (arg_parent == 1) execl("./hackme", NULL);
    }

    return 0;
}

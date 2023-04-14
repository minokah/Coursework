#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define ARRAY_SIZE 300

void part1(int n, int a[]) {
    printf("Elements: ");
    for (int i = 0; i != n; i++) printf("%d ", a[i]);
    printf("\nThere are %d numbers stored\n", n);
    printf("Size of this array (%d) in bytes: %d\n", ARRAY_SIZE, sizeof(a)*n);
}

void part2(int n, int a[]) {
    printf("Elements in reverse order: ");
    for (int i = n; i != 0; i--) printf("%d ", a[i - 1]);
}

void part3(int n, int a[]) {
    int smallest = a[0];
    int index = 0;
    for (int i = 0; i != n; i++) {
        if (a[i] < smallest) {
            smallest = a[i];
        }
    }
    printf("\nSmallest value in array: %d\n", smallest);
}

void part4(int n, int a[]) {
    int sum = 0;
    for (int i = 0; i != n; i++) sum += a[i];
    printf("Sum of values in array: %d\n", sum);
}

void part5(int n, int a[]) {
    printf("Elements but first and last are swapped: ");
    printf("%d ", a[n - 1]);
    for (int i = 1; i != n - 1; i++) printf("%d ", a[i]);
    printf("%d\n", a[0]);
}

void part6(int n, int a[]) {
    printf("Elements of array in ascending order: ");
    while (true) {
        bool swap = false;
        for (int i = 0; i != n - 1; i++) {
            if (a[i] > a[i + 1]) {
                int temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
                swap = true;
            }
        }

        if (!swap) break;
    }
    for (int i = 0; i != n; i++) printf("%d ", a[i]);
}

int main() {
    int integers[ARRAY_SIZE];
    int numbers = 0;
    char nlcheck;
    bool skipRest = false;
    int temp;

    // Get input
    printf("Please enter your integers separated by spaces (invalid characters will stop input): ");
    while (scanf("%d%c", &temp, &nlcheck)) {
        integers[numbers] = temp;
        numbers++;

        // Stop input if not a space or newline
        if (nlcheck != ' ' || nlcheck == '\n') break;
    }

    // Terminate if array is empty
    if (numbers == 0) {
        printf("There is nothing in the array.\n");
        return 0;
    }

    part1(numbers, integers);
    part2(numbers, integers);
    part3(numbers, integers);
    part4(numbers, integers);
    part5(numbers, integers);
    part6(numbers, integers);

    printf("\n");
    
    return 0;
}

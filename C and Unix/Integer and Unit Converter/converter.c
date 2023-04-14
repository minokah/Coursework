// Brandon Luu (bluu8)
// Converter - converts unit to unit

#include <stdio.h>

int main() {
    // print prompts
    printf("Converter\n\n");
    printf("Available conversions:\n");
    printf("1: Kilometers to Miles (K) - Miles to Kilometers (M)\n");
    printf("2: Litres to Gallons (L) - Gallons to Litres (G)\n");
    printf("3: Hectares to Acres (H) - Acres to Hectares (A)\n");
    printf("4: Kilograms to Pounds (K) - Pounds to Kilograms (P)\n");
    printf("5: Quit");

    while (1) {
        int selection;
        char direction;
        float number;

        // get input and handle bad inputs
        printf("\n\nEnter your input: ");
        scanf("\n%d", &selection);

        if (selection < 1 || selection > 5) {
            printf("\nInvalid selection.");
            continue;
        }
        else if (selection == 5) {
            printf("Exiting.\n");
            return 0;
        }

        // input direction and handle bad directions
        printf("Conversion %d selected. Select a direction: ");
        scanf("\n%c", &direction);

        if (selection == 1) {
            if (direction != 'K' && direction != 'M') {
                printf("\nInvalid direction for conversion %d.", selection);
                continue;
            }
        }
        else if (selection == 2) {
            if (direction != 'L' && direction != 'G') {
                printf("\nInvalid direction for conversion %d.", selection);
                continue;
            }
        }
        else if (selection == 3) {
            if (direction != 'H' && direction != 'A') {
                printf("\nInvalid direction for conversion %d.", selection);
                continue;
            }
        }
        else if (selection == 4) {
            if (direction != 'K' && direction != 'P') {
                printf("\nInvalid direction for conversion %d.", selection);
                continue;
            }
        }

        // get float and do conversions.
        printf("\nInput your desired value: ", direction, selection);
        scanf("\n%f", &number);

        if (selection == 1) {
            if (direction == 'K') printf("Your conversion is: %f", number * 0.621371);
            else if (direction == 'M') printf("Your conversion is %f", number / 0.621371);
        }
        else if (selection == 2) {
            if (direction == 'L') printf("Your conversion is: %f", number * 0.264172);
            else if (direction == 'G') printf("Your conversion is %f", number / 0.264172);
        }
        else if (selection == 3) {
            if (direction == 'H') printf("Your conversion is: %f", number * 2.47105);
            else if (direction == 'A') printf("Your conversion is %f", number / 2.47105);
        }
        else if (selection == 4) {
            if (direction == 'K') printf("Your conversion is: %f", number * 2.20462);
            else if (direction == 'P') printf("Your conversion is %f", number / 2.20462);
        }
    }
}
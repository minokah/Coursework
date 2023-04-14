// Brandon Luu (bluu8)
// intToEnglish: converts an integer to it's English format test

#include <stdio.h>

int main()
{
    printf("\nNumber to English Converter\n");

    while (1) {
        int number;
        printf("\nPlease enter a value (1-999, 0 to quit): ");
        scanf("\n%d", &number);
        
        if (number == 0) {
            printf("Exiting.\n\n");
            break;
        }
        else if (number > 1000 || number < 0) {
            printf("That number is not within acceptable range.\n\n");
            continue;
        }
        
        // Check and handle hundreds
        int hundreds = number / 100;
        
        if (hundreds != 0) {
            if (hundreds == 1) printf("one-hundred ");
            else if (hundreds == 2) printf("two-hundred ");
            else if (hundreds == 3) printf("three-hundred ");
            else if (hundreds == 4) printf("four-hundred ");
            else if (hundreds == 5) printf("five-hundred ");
            else if (hundreds == 6) printf("six-hundred ");
            else if (hundreds == 7) printf("seven-hundred ");
            else if (hundreds == 8) printf("eight-hundred ");
            else if (hundreds == 9) printf("nine-hundred ");
            number -= hundreds * 100;
        }
        
        // Check and handle tens
        int tens = number / 10;
        
        if (tens != 0) {
            if (number == 10) printf("ten");
            else if (number == 11) printf("eleven");
            else if (number == 12) printf("twelve");
            else if (number == 13) printf("thirteen");
            else if (number == 14) printf("fourteen");
            else if (number == 15) printf("fifteen");
            else if (number == 16) printf("sixteen");
            else if (number == 17) printf("seventeen");
            else if (number == 18) printf("eighteen");
            else if (number == 19) printf("nineteen");
            else if (tens == 2) printf("twenty");
            else if (tens == 3) printf("thirty");
            else if (tens == 4) printf("forty");
            else if (tens == 5) printf("fifty");
            else if (tens == 6) printf("sixty");
            else if (tens == 7) printf("seventy");
            else if (tens == 8) printf("eighty");
            else if (tens == 9) printf("ninety");
            number -= tens * 10;

            if (tens != 1 && number > 0) printf("-");
        }
        
        // Check and handle ones
        if (tens != 1) {
            if (number == 1) printf("one");
            else if (number == 2) printf("two");
            else if (number == 3) printf("three");
            else if (number == 4) printf("four");
            else if (number == 5) printf("five");
            else if (number == 6) printf("six");
            else if (number == 7) printf("seven");
            else if (number == 8) printf("eight");
            else if (number == 9) printf("nine");
        }

        printf("\n");
    }

    return 0;
}

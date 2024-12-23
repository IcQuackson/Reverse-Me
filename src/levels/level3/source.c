#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void no(void) {
    puts("Nope.");
    exit(EXIT_FAILURE);
}

static void ok(void) {
    puts("Good job.");
}

/*
 * main() function logic:
 *   1. Check first two bytes of user_string:
 *      - The code checks if the first byte is ASCII 50 ('2'),
 *        and the second byte is ASCII 52 ('4').
 *   2. If not matched, call no().
 *   3. A buffer is created to store the converted characters.
 *   4. For each group of 3 characters from user_string, convert them to
 * 	integers and store them into the buffer.
 *   5. 042 = '*'
 *   6. 42042042042042042042042 = "********"
 *   7. Compare buffer with "********". If it matches, print "Good job."
 *      else print "Nope."
 */

int main(void) {
    char user_string[24] = {0};

    printf("Please enter key: ");
    fflush(stdout);

    if (scanf("%23s", user_string) != 1) {
        no();
    }

    /*
     * Check if start of str is 42
     */
    if (user_string[0] != '4' || user_string[1] != '2') {
        no();
    }

    char str[9];
    memset(str, 0, sizeof(str));
    str[0] = '*';

    if (strlen(str) < 8) {
        int writeIndex = 1;      // Next position in 'str'
        int readIndex = 2;       // Next position in 'user_string'
        int inputLen  = strlen(user_string);

		// Loop through the user_string
        while (readIndex < inputLen) {
            char temp[4]; // 3 characters + null-terminator
			temp[0] = user_string[readIndex];
			temp[1] = user_string[readIndex + 1];
			temp[2] = user_string[readIndex + 2];
			temp[3] = '\0';

            str[writeIndex] = (char)atoi(temp);
            writeIndex++;

            if (strlen(str) >= 8) {
                break;
            }

            readIndex += 3;
            inputLen = strlen(user_string);
        }
        str[writeIndex] = '\0';
    }

    if (strcmp(str, "********") == 0) {
        ok();
    } else {
        no();
    }

    return 0;
}

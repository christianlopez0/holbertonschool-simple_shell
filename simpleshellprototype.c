#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100

int main() {
    char command[MAX_COMMAND_LENGTH];
    int index = 0;
    char ch;

    while (1) {

        printf("simple_shell> ");

        while (1) {
            ch = getchar();

            if (ch == EOF || ch == '\n') {
                break;
            }

            if (index < MAX_COMMAND_LENGTH - 1) {
                command[index++] = ch;
            } else {
                fprintf(stderr, "Command is too long. Maximum length is %d\n", MAX_COMMAND_LENGTH - 1);
                return 1;
            }
        }

        command[index] = '\0';
        index = 0;

	
        if (ch == EOF) {
            printf("\nExiting the shell.\n");
            break;
        }
	
        if (system(command) == -1) {
            fprintf(stderr, "Error executing command: %s\n", command);
        }
    }

    return 0;
}


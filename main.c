#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFF 512
#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

int get_line(char* prompt, char* buffer, int buffer_size);

int main(int argc, char** argv)
{
    // argc not needed
    (void)argc;

    // Prompt
    char* prompt = "$ ";

    // Main run loop
    for (;;) {
        char buffer[MAX_BUFF];
        int input_status = get_line(prompt, buffer, MAX_BUFF);
        if (input_status == NO_INPUT)
            continue;
        // Hope input is never longer than MAX_BUFF

        //Check if command is to exit
        if (!strcmp(buffer, "exit"))
            exit(0);

        // Fork to run command
        pid_t cpid = fork();

        // Fork failed
        if (cpid < 0) {
            printf("%s: Fork failed.\n", argv[0]);
            exit(1);
        } else if (cpid > 0) {
            // Parent process
            int child_status;
            wait(&child_status);
        } else {
            // Child process
            int status = system(buffer);
            exit(status);
        }
    }
}

int get_line(char* prompt, char* buffer, int buffer_size)
{
    int ch, extra;

    // Get line with buffer overflow protection.
    if (prompt != NULL) {
        printf("%s", prompt); // Print prompt
        fflush(stdout);
    }
    if (fgets(buffer, buffer_size, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buffer[strlen(buffer) - 1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buffer[strlen(buffer) - 1] = '\0';
    return OK;
}

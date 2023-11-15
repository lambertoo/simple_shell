#include "main.h"

/**
 * run_shell - Run the main loop of the shell
 */
void run_shell(void)
{
    char *command = NULL;
    size_t len = 0;

    while (1)
    {
        display_prompt();

        /* Read a line from the user */
        if (getline(&command, &len, stdin) == -1)
        {
            if (feof(stdin))
            {
                /* Handle Ctrl+D */
                printf("\nExiting simple_shell.\n");
                break;
            }
            else
            {
                /* Some other error */
                perror("getline");
                _exit(EXIT_FAILURE);
            }
        }

        /* Remove newline character */
        command[strcspn(command, "\n")] = '\0';

        /* Execute the command */
        execute_command(command);
    }

    free(command);
}


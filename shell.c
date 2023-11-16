/* shell.c */
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;  /* Declare environ */

/**
 * run_shell - Function to run the shell.
 */
void run_shell(void)
{
    char *line;
    size_t len = 0;

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        getline(&line, &len, stdin);

        /* Remove newline character from the input */
        line[strcspn(line, "\n")] = '\0';

        if (fork() == 0)
        {
            char *args[] = {"/bin/sh", "-c", NULL, NULL};
            args[2] = line;
            execve(args[0], args, environ);
            perror("execve");  /* Print an error message if execve fails */
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
}


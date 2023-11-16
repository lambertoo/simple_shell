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
        char *args[1024];  /* Adjust the size as needed */
        int i = 0;

        write(STDOUT_FILENO, "$ ", 2);
        getline(&line, &len, stdin);

        /* Remove newline character from the input */
        line[strcspn(line, "\n")] = '\0';

        if (fork() == 0)
        {
            /* Tokenize the input line into arguments */
            char *token = strtok(line, " ");
            while (token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            /* Check if the command exists in the PATH */
            if (access(args[0], X_OK) == 0)
            {
                execve(args[0], args, environ);
                perror("execve");  /* Print an error message if execve fails */
                exit(EXIT_FAILURE);
            }
            else
            {
                /* Print an error message if the command doesn't exist */
                fprintf(stderr, "%s: command not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            wait(&status);

            /* Check if the command is the built-in "exit" */
            if (strcmp(args[0], "exit") == 0)
            {
                /* Free allocated memory and exit the shell */
                free(line);
                exit(EXIT_SUCCESS);
            }
        }
    }

    free(line);
}


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

            /* Check if the command is the built-in "env" */
            if (strcmp(args[0], "env") == 0)
            {
                /* Print the current environment variables */
                char **env;
                for (env = environ; *env != NULL; env++)
                {
                    printf("%s\n", *env);
                }

                /* Free allocated memory and exit the child process */
                free(line);
                exit(EXIT_SUCCESS);
            }

            /* Check if the command is the built-in "cd" */
            if (strcmp(args[0], "cd") == 0)
            {
                if (args[1] == NULL)
                {
                    fprintf(stderr, "cd: missing argument\n");
                    exit(EXIT_FAILURE);
                }

                if (chdir(args[1]) != 0)
                {
                    perror("chdir");
                    exit(EXIT_FAILURE);
                }

                /* Free allocated memory and exit the child process */
                free(line);
                exit(EXIT_SUCCESS);
            }

            /* Check if the command is the built-in "exit" */
            if (strcmp(args[0], "exit") == 0)
            {
                /* Free allocated memory and exit the shell */
                free(line);
                exit(EXIT_SUCCESS);
            }

            /* Check if the command exists in the PATH */
            if (access(args[0], X_OK) != 0)
            {
                /* Search for the command in the directories specified in PATH */
                char *path_env = getenv("PATH");
                char *path_copy = strdup(path_env);
                char *dir = strtok(path_copy, ":");

                while (dir != NULL)
                {
                    char path[1024];
                    snprintf(path, sizeof(path), "%s/%s", dir, args[0]);
                    if (access(path, X_OK) == 0)
                    {
                        execve(path, args, environ);
                        perror("execve");  /* Print an error message if execve fails */
                        exit(EXIT_FAILURE);
                    }
                    dir = strtok(NULL, ":");
                }

                /* Free allocated memory and exit the child process */
                free(path_copy);
                fprintf(stderr, "%s: command not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                execve(args[0], args, environ);
                perror("execve");  /* Print an error message if execve fails */
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            wait(&status);
        }
    }

    free(line);
}


/* shell.c */

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * print_prompt - Displays the shell prompt
 */
void print_prompt(void)
{
    printf("$ ");
}

/**
 * run_shell - Runs the main shell loop
 */
void run_shell(void)
{
    char *command = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    pid_t wpid;

    while (1)
    {
        print_prompt();

        /* Read command from the user */
        read = getline(&command, &len, stdin);

        /* Check for end of file (Ctrl+D) */
        if (read == -1)
        {
            printf("\n");
            break;
        }

        /* Remove newline character */
        if (command[read - 1] == '\n')
            command[read - 1] = '\0';

        /* Fork a new process */
        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) /* Child process */
        {
            /* Execute the command */
            char **argv = malloc(2 * sizeof(char *));
            if (argv == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            argv[0] = command;
            argv[1] = NULL;

            if (execve(command, argv, NULL) == -1)
            {
                perror("execve");
                exit(EXIT_FAILURE);
            }

            /* Free allocated memory */
            free(argv);
        }
        else /* Parent process */
        {
            /* Wait for the child process to complete */
            wpid = waitpid(pid, NULL, 0);
            if (wpid == -1)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Free allocated memory */
    free(command);
}


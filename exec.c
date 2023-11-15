#include "main.h"

/**
 * execute_command - Execute a
 * command in a child process
 * @command: The command to execute
 */
void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		char **args = malloc(2 * sizeof(char *));

		if (args == NULL)
		{
			perror("malloc");
			_exit(EXIT_FAILURE);
		}

		args[0] = command;
		args[1] = NULL;

		execve(command, args, NULL);

		/* If exec fails */
		perror("execve");
		free(args);
		_exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		int status;

		waitpid(pid, &status, 0);
	}
}

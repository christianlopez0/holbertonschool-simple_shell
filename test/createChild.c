#include "shell.h"

int createChild(char *input, char **paths)
{
	pid_t pid;
	int status;
	char **command;

	if (strlen(input) > 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
		}
		else if (pid == 0)
		{
			command = find_executable(input, paths);
			executeCommand(command);
			free(command);
			for (int i = 0; paths[i]; i++)
				free(paths[i]);
			free(input);
			exit(EXIT_SUCCESS);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}
	free(command);
	for (int i = 0; command[i]; i++)
		free(command[i]);
	free(input);
	return (status);
}
#include "shell.h"

void executeCommand(char **command)
{
	char *full_path;

	if (command[0] == NULL)
		return;
	
	full_path = getenv("PATH");
	execve(command[0], command, &full_path);
	perror("execve");
	exit(EXIT_FAILURE);
}

char **find_executable(char *command, char *path_list[])
{
	char *temp = strtok(command, " ");
	int arg_index = 0, i = 0;
	
	char **result = malloc(MAX_PATHS * sizeof(char *));
	if (result == NULL)
		{
			fprintf(stderr, "Error: Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
	for (i = 0; i < MAX_PATHS; i++)
		result[i] = NULL;
	while (temp != NULL)
		{
			if (strchr(temp, '/') != NULL)
				{
					if (access(temp, X_OK) == 0)
						{
							result[0] = strdup(temp);
							if (result[0] == NULL)
								{
									fprintf(stderr, "Error: Memory allocation failed.\n");
									exit(EXIT_FAILURE);
								}
						}
					else
						{
							free(result[0]);
							result[0] = NULL;
						}
				}
			else
				{
					for (int i = 0; i < MAX_PATHS && path_list[i] != NULL; i++)
						{
							char child_full_path[MAX_PATH];
							child_full_path[0] = '\0';
							snprintf(child_full_path, MAX_PATH, "%s/%s", path_list[i], temp);
							if (access(child_full_path, X_OK) == 0)
								{
									result[0] = strdup(child_full_path);
									if (result[0] == NULL)
										{
											fprintf(stderr, "Error: Memory allocation failed.\n");
											exit(EXIT_FAILURE);
										}
									break;
								}
						}
				}
			result[arg_index + 1] = strdup(temp);
			if (result[arg_index + 1] == NULL)
				{
					fprintf(stderr, "Error: Memory allocation failed.\n");
					exit(EXIT_FAILURE);
				}
			temp = strtok(NULL, " ");
			arg_index++;
		}
	return result;
}

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

char **getPaths()
{
	char *pathList, *token;
	char **paths;
	int pathIndex = 0;

	paths = malloc(MAX_PATHS * sizeof(char *));
	if (paths == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	pathList = getenv("PATH");
	if (pathList == NULL) 
	{
		fprintf(stderr, "Error: PATH environment variable not set.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(pathList, ":");
	while (token != NULL && pathIndex < sizeof(paths) / sizeof(paths[0]))
	{
		paths[pathIndex] = malloc(strlen(token) + 1);
		strcpy(paths[pathIndex], token);
		pathIndex++;
		token = strtok(NULL, ":");
	}

	return paths;
}

int main()
{
	int i;
	char *input;
	char **paths;
	size_t inputsize;
	
	paths = getPaths();
	if (paths == NULL)
	{
		return (1);
	}
	while (1)
	{
		if (isatty(0))
		{
			if (getline(&input, &inputsize, stdin) == -1)
			{
				printf("\n");
				free(input);
				return (1); //could be exit
			}
			input[strcspn(input, "\n")] = '\0';
			createChild(input, paths);
		}
		else
		{
			printf("$ ");
			fflush(stdout);
			if (getline(&input, &inputsize, stdin) == -1)
			{
				printf("\n");
				free(input);
				return (1); // could be exit
			}
			input[strcspn(input, "\n")] = '\0';
			createChild(input, paths);
		}
		free(input);
	}
	for (int i = 0; paths[i]; i++)
		free(paths[i]);
	return (0);
}
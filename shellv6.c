#include "shell.h"

int main(void)
{
	int status, token_count;
	char *input;
	size_t input_size;
	char **tokens;
	pid_t pid;

	while (1)
	{
		input = NULL;
		input_size = 0;

		if (isatty(STDIN_FILENO))
		{
			printf("$ ");
			fflush(stdout);
		}
		if (getline(&input, &input_size, stdin) == -1)
		{
			printf("\n");
			free(input);
			break;
		}
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) > 0)
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
			}
			else if (pid == 0)
			{
				tokens = tokenize_input(input, &token_count);
				execute_command(tokens, get_path_list());
				free(tokens);
				free(input);
				exit(EXIT_SUCCESS);
			}
			else
			{
				waitpid(pid, &status, 0);
			}
		}
		free(input);
	}
	return (0);
}

char **tokenize_input(char *input, int *token_count)
{
	char *token;
	char **tokens = malloc(MAX_PATH * sizeof(char *));

	if (tokens == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	*token_count = 0;
	token = strtok(input, " ");
	while (token != NULL)
	{
		tokens[(*token_count)++] = strdup(token);
		token = strtok(NULL, " ");
	}
	tokens[*token_count] = NULL;

	if (*token_count == 0)
	{
		free(tokens);
		return (NULL);
	}

	return (tokens);
}

PathNode *get_path_list()
{
	char *path_var = getenv("PATH");
	char *path = strtok(path_var, ":");
	PathNode *head = NULL;
	PathNode *current = NULL;
	PathNode *node = NULL;

	if (path_var == NULL)
	{
		fprintf(stderr, "Error: Unable to retrieve PATH environment variable\n");
		exit(EXIT_FAILURE);
	}
	while (path != NULL)
	{
		node = malloc(sizeof(PathNode));
		if (node == NULL)
		{
			fprintf(stderr, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		strncpy(node->path, path, MAX_PATH - 1);
		node->path[MAX_PATH - 1] = '\0';
		node->next = NULL;
		if (head == NULL)
		{
			head = node;
			current = node;
		}
		else
		{
			current->next = node;
			current = node;
		}
		path = strtok(NULL, ":");
	}
	return (head);
}

void execute_command(char **argv, PathNode *path_list)
{
	char full_path[MAX_PATH];

	if (argv[0] == NULL)
	{
		return;
	}
	if (find_executable(argv[0], path_list, full_path))
	{
		execve(full_path, argv, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", argv[0]);
	}
}

int find_executable(char *command, PathNode *path_list, char *full_path)
{
	char child_full_path[MAX_PATH];

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
		{
			strncpy(full_path, command, MAX_PATH - 1);
			full_path[MAX_PATH - 1] = '\0';
			return (1);
		}
		else
		{
			full_path[0] = '\0';
		}
	}
	else
	{
		while (path_list != NULL)
		{
			child_full_path[0] = '\0';
			snprintf(child_full_path, MAX_PATH, "%s/%s", path_list->path, command);
			if (access(child_full_path, X_OK) == 0)
			{
				strncpy(full_path, child_full_path, MAX_PATH - 1);
				full_path[MAX_PATH - 1] = '\0';
				return (1);
			}
			path_list = path_list->next;
		}
	}
	if (access(command, X_OK) == 0)
	{
		strncpy(full_path, command, MAX_PATH - 1);
		full_path[MAX_PATH - 1] = '\0';
		return (1);
	}
	return (0);
}

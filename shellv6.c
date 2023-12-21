#include "shell.h"

int main(void)
{
	int token_count, i = 0;
	char *input, *full_path;
	size_t input_size;
	char **tokens;
	PathNode head = *get_path_list();

	while (1)
	{
		input = NULL;
		input_size = 0;
		token_count = 0;

		if (isatty(STDIN_FILENO))
		{
			printf("$ ");
			fflush(stdout);
		}
		if (getline(&input, &input_size, stdin) == -1)
		{
			free(input);
			break;
		}
		full_path = malloc(MAX_PATH);
		if (full_path == NULL)
		{
			fprintf(stderr, "Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		input[strcspn(input, "\n")] = '\0';
		tokens = tokenize_input(input, &token_count);
		if (find_executable(tokens[0], &head, full_path))
		{
			execute_command(tokens, full_path);
		}
		else
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", tokens[0]);
		}
		for (i = 0; i < token_count; ++i) {
			free(tokens[i]);
		}
		free(tokens);
		free(full_path);
		free(input);
	}
	while (head.next != NULL)
	{
		PathNode *temp = &head;
		head = *head.next;
		free(temp);
	}
	return (0);
}

char **tokenize_input(char *input, int *token_count)
{
	char *token;
	char **tokens = malloc(MAX_PATH *sizeof(char *));

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

void execute_command(char **command, char *full_path)
{
	int status = 0;
	pid_t pid;

	if (command[0] == NULL)
	{
		return;
	}
	if (strlen(command[0]) > 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
		}
		else if (pid == 0)
		{
			execve(full_path, command, NULL);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
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
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wformat-truncation"
			snprintf(child_full_path, MAX_PATH, "%s/%s", path_list->path, command);
			#pragma GCC diagnostic pop
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

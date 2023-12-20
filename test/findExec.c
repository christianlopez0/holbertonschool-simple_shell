#include "shell.h"

#define MAX_PATH 256
#define MAX_PATHS 10

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
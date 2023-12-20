#include "shell.h"

char **getPaths()
{
	char *pathList, *token;
	char **paths;
	int pathIndex = 0;

	paths = malloc(maxString * sizeof(char *));
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int i;
	char **paths;
	char *path_var, *token;
	
	path_var = getenv("PATH");
	printf("%s \n\n", path_var);
	
	path_var = strtok(path_var, ":");
	while (path_var != NULL && i < sizeof(paths) / sizeof(paths[0]))
	{
		paths[i] = strdup(path_var);
		i++;
		path_var = strtok(NULL, ":");
	}
	
	for (int i = 0; paths[i] != NULL; i++) {
		printf("Path %d: %s\n", i + 1, paths[i]);
	}
	
	// Free the allocated memory
	for (int i = 0; paths[i] != NULL; i++) {
		free(paths[i]);
	}
}



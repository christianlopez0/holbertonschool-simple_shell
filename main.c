#include "shell.h"

int main()
{
	int i;
	char *input;
	char **paths;
	size_t inputsize;
	
	paths = getPaths;
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
	for (int i = 0; paths[i] != NULL; i++)
		free(paths[i]);
	return (0);
}
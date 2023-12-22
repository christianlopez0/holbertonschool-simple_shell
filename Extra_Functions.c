#include "shell.h"

char *IOHandling(char *input)
{
	input = NULL;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
	size_t input_size;
	#pragma GCC diagnostic pop
	
	if (isatty(STDIN_FILENO))
	{
		printf("$ ");
		fflush(stdout);
	}
	if (getline(&input, &input_size, stdin) == -1)
	{
		free(input);
		return (input);
	}
	input[strcspn(input, "\n")] = '\0';
	return (input);
}

int isAllWhite(char *input)
{
	size_t whitespaces = 0, indx;

	for (indx = 0; indx < strlen(input); indx++)
	{
		if (input[indx] == ' ')
		{
			whitespaces++;
		}
	}
	if (strlen(input) == whitespaces)
	{
		return (1);
	}
	return (0);
}
int implementExitEnv(char *input)
{
	char **env;

	if (strcmp(input, "exit") == 0)
	{
		return (1);
	}
	
	if (strcmp(input, "env") == 0)
	{
		env = environ;
		while (*env != NULL)
		{
			printf("%s\n", *env);
			env++;
		}
	}
	return (0);
}
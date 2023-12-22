#include "shell.h"

char *IOHandling(char *input)
{
	input = NULL;
	size_t input_size = 0;

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

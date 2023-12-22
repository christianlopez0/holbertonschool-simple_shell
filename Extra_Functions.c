#include "shell.h"

/**
* prompt - prints the dollar sign and checks its atty
*/
void prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("$ ");
		fflush(stdout);
	}
}


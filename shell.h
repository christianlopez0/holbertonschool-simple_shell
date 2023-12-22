#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH 2049

extern char **environ;

typedef struct PathNode{
	char path[MAX_PATH];
	struct PathNode *next;
} PathNode;

char **tokenize_input(char *input, int *token_count);
PathNode *get_path_list();
void execute_command(char **tokens, PathNode *path_list);
int find_executable(char *command, PathNode *path_list, char *full_path);
char *IOHandling(char *input);
char *isAllWhite(char *input);
int implementExitEnv(char *input);
void prompt();

#endif

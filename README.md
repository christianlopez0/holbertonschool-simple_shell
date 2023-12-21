
![1_lqEaA1-6gGQhdLS3k8X0xw](https://user-images.githubusercontent.com/31927278/182706961-e087c64e-9d7b-40db-a931-67009dc34089.gif)

## Introduction
This repository is a Holberton School Project. The school project consisted in writing a shell like `sh` (Bourne Shell) by Stephen Bourne , in `C`.
using a limited number of standard library functionsusing a limited number of standard library functions.
### General

- `README file`, at the root of the folder of the project is mandatory.
- Allowed editors: `vi`, `vim`, `emacs`
- Compiler;
  - Ubuntu 20.04 LTS using gcc.
  - Using the options `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Coding style;
  - Betty Style.
- Shell should not have any memory leaks.
- No more than 5 functions per file.
- All header files should be include guarded.


## Compilation

* gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Features
- Prints a prompt and waits for a command from the user.
- Creates a child process in which the command is checked.
- Checks for built-ins, aliases in the PATH, and local executable programs.
- The child process is replaced by the command, which accepts arguments.
- When the command is done, the program returns to the parent process and prints the prompt.
- The program is ready to receive a new command.
- To exit: press `Ctrl-D` or enter "`exit`" (with or without a status).
- Works also in non interactive mode.




## Authors
* Gabriel Velez
* Christian Lopez

# SimpleShell

The program implements a basic shell in command line. It was implemented on linux.

Key Features
- Programmed in C
- Uses fork function
- Uses exec function
- Uses pipe function

This simple shell supports single command with arguments and single command without arguments.

It supports I/O redirection. Ex: sort > t.txt > t2.txt 

It can also use multiple pipes. (ls -l | sort | more)

It also supports a history command to show the last ten commands issued.

Ctrl C and 'exit' will terminate the shell appropriately.

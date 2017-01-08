# SimpleShell

The program implement as basic shell in command line. It was implement on and for linux.

Key features
-Programmed in C
-uses fork function
-uses exec function
-use pipe function

This simple shell supports single command arguments and single commands without arguments.
It supports I/O redirection. Ex: sort > t.txt > t2.txt 
It can also use multiple pipes. (ls -l | sort | more)
It also supports a history command to show the last ten commands issued.
Ctrl C and exit will terminate the shell appropriately.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
#define CMD_MAX 256
#define historyNum 10

void getTokenFromLine(char *tokens[], int *i, char *line,char *deliminater, bool flag);
void getToken(char *tokens[], int *i, char *line,char *deliminater, bool flag);
void e(char *tokens[], char *line);
void sig_handler();


int main(void)
{
//INITIALIZE VARIABLES
char *tokens[CMD_MAX];
char *progs[CMD_MAX];
char line[CMD_MAX];
char tokenHistory[historyNum][CMD_MAX];
char *s;
int size = 0;
bool fill;
int i,h,start, end, temp;
int status;
//SIGNAL AND SIGNAL HANDLER FOR CTRL-C
signal(SIGINT,sig_handler);


//LOOP FOR COMMAND LINE
do{

//FLUSH USER INPUT IN LINE WITH NULL CHARACTER EACH TIME BEFORE RESUSING
strncpy(line,"\0",CMD_MAX);
//PRINT name>
printf("andy> ");
//PROMPT USER FOR INPUT IF (True)
getToken(tokens,&size,line," \n\r\t", true);
getToken(progs,&size,line,"|><\n\r\t", false);

//IF USER INPUT IS NULL, ADD BLANK STRING TO TOKEN HISTORY, AND INCREMENT TOKEN HISTORY ARRAY 
if(tokens[0] == NULL)
{
//add to history
strncpy(tokenHistory[end]," ",1);
//increment head
end++;

//NOTE:PART OF HISTORY, USES CIRCULAR ARRAY DATASTRUCTURE AND RESETS BACK TO 0 WHEN POINTER IS AT 10
if(end == 10)
{
end = 0;
fill = true;
}
//NOTE:PART OF HISTORY, CHECKS IF THE CIRCULAR ARRAY IS FILLED ATLEAST ONCE 
if(fill == true) 
{
start++;
if (start == 10)
start = 0;
}
}
else
{
//ELSE ADD THE LINE, TO TOKEN HISTORY
strncpy(tokenHistory[end],line,sizeof(line));
//increment head
end++;

//NOTE:PART OF HISTORY: ALSO CHECK FOR CIRCULAR ARRAY HEAD IF FULL
if(end == 10)
{
end = 0;
fill = true;
}
//NOTE: check if loop if full, then must increment start as well
if(fill == true) 
{
start++;
if (start == 10)
start = 0;
}
//IF USER INPUT IS NOT NULL, CHECK IF EXIT, HISTORY OR CTRL-C IS TYPED INSTEAD THEN PERFORM THE APPROPRIATE ACTION
if(strncmp(tokens[0],"exit",4) == 0 || strncmp(tokens[0],"Ctrl-C",6) == 0)
{
//IF EXIT OR CTRL C, EXIT
return 0;
}
else if(strncmp(tokens[0],"history", 7) == 0)
{
//ITERATE THROUGH CIRCULAR ARRAY IN TOKENHISTORY
temp = start;
while(temp != end)
{
printf("temp%d|end%d|item:%s\n",temp,end,tokenHistory[temp]);
temp++;
if(temp == 10){
temp = 0;
}
}

}
//ELSE IF NOT NULL, OR BUILTIN THEN TRY TO RUN PROGRAM
else
{
//GET USER INPUT BY SEPERATED BY PIPE OR REDIRECTION INSTEAD
getTokenFromLine(progs,&size,line,"|><\n\r\t", false);
//EXECUTE THE APPROPRIATE COMMAND AND HANDLE PIPE AND REDIRECTION
e(progs,tokenHistory[end-1]);
}




}

}while(1);

return 0;
}
//SIGNAL HANDLER FOR CTRL-C , exit(1);
void sig_handler(){
printf("\nLater\n");
exit(1);
}

//FUNCTION TO GET RETURN TOKENS FROM LINE SEPERATED BY DELIMINATER, FLAG ASK TO PROMPT USER OR NOT
void getToken(char *tokens[], int *i, char *line,char *deliminater, bool flag)
{
size_t bufsize = 0;
char buffer[CMD_MAX];
char *temp = NULL;
//Get line
if(flag){
getline(&temp,&bufsize,stdin);
strcpy(line,temp);
line[strlen(line)-1] = '\0';
}
else
{
strncpy(buffer,line,sizeof(line));
temp = line;
}
(*i) = 0;

//Get Token
tokens[(*i)] = strtok(temp,deliminater);
do{
(*i)++;
tokens[(*i)] = strtok(NULL, deliminater);

}while(tokens[(*i)] != NULL);
if(!flag)
{
strncpy(line,buffer,sizeof(line));
}
}

//FUNCTION TO GET RETURN TOKENS FROM LIN SEPERATED BY DELIMINATER, FLAG ASK TO PROMPT USER OR NOT
void getTokenFromLine(char *tokens[], int *i, char *line,char *deliminater, bool flag)
{
size_t bufsize = 0;
char *temp = NULL;
//Get line
if(flag){
getline(&temp,&bufsize,stdin);
strcpy(line,temp);
}
else
{
temp = line;
}
(*i) = 0;
//Get Token
tokens[(*i)] = strtok(temp,deliminater);
do{
(*i)++;
tokens[(*i)] = strtok(NULL, deliminater);
}while(tokens[(*i)] != NULL);
}
//FUNCTION 
void e(char *tokens[], char *line)
{
//INITIALIZE VARIABLES
pid_t pid;
pid_t childpid;
int status;
int size = 0;
int fd[2];
int fd2[2];
int i;
char delim[1];
int g;
char *deliminator;
char c[] = "|<>";
char* progsToken2[CMD_MAX];
char* progsToken3[CMD_MAX];
int in;
int ot;
char* progsToken[CMD_MAX];

//GET DELIMINATOR FROM LINE , CHECK IF PIPE , REDIRECTION ETC... (ASSUMES THAT REDIRECTION AND PIPE DO NOT MIX)
deliminator = strpbrk(line,c);
//IF NO PIPE OR REDIRECTION FORK CHILD AND EXEC
if(deliminator == NULL)
{


//SEPERATE USER INPUT INTO TOKENS
getTokenFromLine(progsToken,&size,tokens[i]," \n\r\t",false);
if((pid = fork()) == 0)
{
if((execvp(progsToken[i],progsToken)) < 0)
{
perror("execErr");
printf("main: errno is %d\n",errno);
}
exit(EXIT_FAILURE);
}
else if (pid > 0)
{

wait(status);

}
else
{

perror("fork err");
}

}
//ELSE IF REDIRECTION WITH < THEN REDIRECT AND CHECK FOR >
else if(*deliminator == '<')
{

//GET FIRST SET OF TOKENS TO EXECUTE
getTokenFromLine(progsToken,&size,tokens[0]," \n\r\t",false);

//FORK 
if((pid = fork())== 0)
{
//GET REDIRECTION 1 
if(tokens[1] != NULL)
{

getTokenFromLine(progsToken2,&size,tokens[1]," \n\r\t",false);
in = open(*progsToken2,O_RDONLY);
dup2(in, 0);
close(in);
}
//GET REDIRECTION 2 
if(tokens[2] != NULL)
{

getTokenFromLine(progsToken3,&size,tokens[2]," \n\r\t",false);
ot = open(*progsToken3,O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
dup2(ot,1);
dup2(ot, 2);
close(ot);
}
//EXECUTE
if(execvp(progsToken[0],progsToken) < 0)
{
perror("execErr");
printf("main: errno is %d\n",errno);
}

exit(EXIT_FAILURE);
}
else if (pid > 0)
{

wait(status);

}
else
{

perror("fork err");
}


}
//ELSE IF REDIRECTION WITH > THEN REDIRECT AND CHECK FOR <
else if (*deliminator == '>')
{
//GET FIRST SET OF TOKENS TO EXECUTE
getTokenFromLine(progsToken,&size,tokens[0]," \n\r\t",false);
//FORK
if((pid = fork())== 0)
{


 //GET REDIRECTION 1 
if(tokens[1] != NULL)
{

getTokenFromLine(progsToken2,&size,tokens[1]," \n\r\t",false);
ot = open(*progsToken2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
dup2(ot, 1);
dup2(ot, 2);
close(ot);
}
//GET REDIRECTION 2 
if(tokens[2] != NULL)
{

getTokenFromLine(progsToken3,&size,tokens[2]," \n\r\t",false);
in = open(*progsToken3,O_RDONLY);
dup2(in,0);
close(in);
}
//EXECUTE
if((execvp(progsToken[0],progsToken)) < 0)
{
perror("execErr");
printf("main: errno is %d\n",errno);
}
exit(EXIT_FAILURE);

}
else if (pid > 0)
{

wait(status);

}
else
{

perror("fork err");
}





}
//ELSE CHECK FOR PIPE
else if(*deliminator == '|')
{

//CREATE PIPE
if (pipe(fd) < 0){
perror("fatal error");
exit(1);
}

//FORK
if((pid = fork()) < 0)
{
perror("forkErr");
}

 if(pid == 0)
  {
	//FORK
  pid = fork();


  if(pid == 0){

  close(fd[1]);
	//SET UP THE PIPE FOR CHILD AND EXECUTE
  if (dup2(fd[0], STDIN_FILENO) < 0){
  perror("can't dup");
  exit(1);
  }
  getTokenFromLine(progsToken,&size,tokens[1]," \n\r\t",false);
  status = execvp(progsToken[0],progsToken);


  if (status < 0){
  perror("parent: exec problem");

  exit(1);
  }

  exit(EXIT_FAILURE);
  }
  else
  {
  //SET UP THE PIPE AND EXECUTE
  close(fd[0]);
  if (dup2(fd[1], STDOUT_FILENO) < 0){
  perror("can't dup");
  exit(1);
  }
  getTokenFromLine(progsToken,&size,tokens[0]," \n\r\t",false);
  status = execvp(progsToken[0],progsToken);

exit(EXIT_FAILURE);
  }
  exit(EXIT_FAILURE);
  }

  else
  {
  wait(status);
  }
  close(fd[0]);
  close(fd[1]);

}
}



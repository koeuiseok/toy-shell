///////////////////////////////////
//   2021 / 05 / 23 car-rtos     
//   HW : Make Toy-shell        
//   20161890 고의석              
///////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN_LINE    100

char userName[MAX_LEN_LINE];
int nameLen;

#define C_NRML "\033[0m"
#define C_BLCK "\033[30m"
#define C_RED  "\033[31m"
#define C_GREN "\033[32m"
#define C_YLLW "\033[33m"
#define C_BLUE "\033[34m"

 
void error_handling(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int init()
{
    char* s;

    printf("********************************\n");
    printf("*  Welcome to euiseok's shell  *\n");
    printf("*   20161890 koeuiseok's HW    *\n");
    printf("*          hello ^&^           *\n");
    printf("********************************\n");

    printf("Input User Name : ");
    
    s = fgets(userName, MAX_LEN_LINE, stdin);
    if (s == NULL) {
        error_handling("fgets error");
    }
    nameLen = strlen(userName);
    userName[nameLen - 1] = ' ';

    return 0;
}

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    

    pid_t pid, cpid;
    int flag=0;
    ret = init();
    
    while (true) {
        char *s;
        char cwd[256];
        char tmp[256];
        char *tmp1;
        char *tmp2;

        int len; 

        flag = 0;
        getcwd(cwd, sizeof(cwd));
        
        printf("%s%s@host :%s $ %s", C_GREN,userName, cwd, C_NRML);
        s = fgets(command, MAX_LEN_LINE, stdin);
        args[0] = command;
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        if (!strcmp(command, "exit\n") || !strcmp(command, "Q\n"))
        {
            break;
        }
        else if (!strcmp(command, "cwd\n"))
        {        
            printf("Now Directory in here : %s\n", cwd);
            continue;
        }
        else if (!strcmp(command, "help\n"))
        {
            printf("****************************************************************\n");
            printf("**                     How to use this shell                  **\n");
            printf("**             if you want to execute your program            **\n");
            printf("**              you will write \"./program_name \"              **\n");
            printf("**             if you want to execute shell command           **\n");
            printf("**             you will write only \"shell_command \"           **\n");
            printf("****************************************************************\n");
            continue;
        }


        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        if (!strncmp(command, "./", 2) ) {
            args[0] = command;
        }
        else {
            sprintf(tmp, "/bin/%s", command);
            tmp1 = strtok(tmp, " ");
            tmp2 = strtok(NULL, " ");
            args[0] = tmp1;
        
        }

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) { 
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else { 
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }

    return 0;
}

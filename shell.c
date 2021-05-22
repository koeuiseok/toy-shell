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

    ret = init();
    
    while (true) {
        char *s;
        char cwd[256];
        int len;
        
        getcwd(cwd, sizeof(cwd));

        printf("%s@ :%s $ ", userName, cwd);
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        if (!strcmp(command, "exit\n") || !strcmp(command, "Q\n"))
        {
            break;
        }
        if (!strcmp(command, "cwd\n"))
        {        
            printf("Now Directory in here : %s\n", cwd);
            continue;
        }


        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }

    return 0;
}

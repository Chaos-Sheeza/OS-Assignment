#include <time.h>
#include <zconf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include "ExterComm.h"

pid_t pid;

void exComm(char *arg[], int argnum)
{
    int     check = 0;
    char    *path,
            *buf = malloc(255),
            *searchpath;
    path = strdup(getenv("PATH"));
    searchpath = strtok(path, ":");
    while (searchpath != NULL) {
        //printf("Path:%s\n", path);
        //printf("Search:%s\n", searchpath);
        strcpy(buf, searchpath);
        strcat(buf, "/");
        strcat(buf, arg[0]);
        if (access(buf, X_OK) == 0) {
            check = 1;
            pid = fork();
            if(pid ==0)
            {
                execv(buf, arg);
                exit(0);
            }
            else if (pid > 0)
            {
                int stat;
                waitpid(pid, &stat, WUNTRACED);
            }
        }
        if (check==1) {
            break;
        }
        searchpath = strtok(NULL, ":");
    }
    if (check==0) {
        printf("Function does not exist.\n");
    }
}

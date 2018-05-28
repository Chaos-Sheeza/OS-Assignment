#include "InterComm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linenoise-master/linenoise.h"

int     tokenindex2;

// Main internal commands function.
void inComm(char *arg[], int argnum)
{
    if (strcmp(arg[0], "exit") == 0)
    {
        exitFun(arg);
    }
    else if (strcmp(arg[0], "print") == 0)
    {
        printFun(arg, argnum);
    }
    else if (strcmp(arg[0], "chdir") == 0)
    {
        changeDir(arg);
    }
    else if (strcmp(arg[0], "all") == 0)
    {
        printAll(arg);
    }
    else if (strcmp(arg[0], "source") == 0)
    {
        sourceComm(arg);
    }
}

// Exit command function.
void exitFun(char *arg[])
{
    // Exits loop when input is exit.
    if (arg[1] == NULL)
    {
        printf("Thank you for using this shell.\n");
        // Unclear what to output.
        setenv("EXITCODE", "EXIT_SUCCESS", 1);
        exit(0);
    }
    else
    {
        printf("Exit requires no arguments");
        setenv("EXITCODE", "EXIT_FAILURE", 1);
    }
}

// Print command function.
void printFun(char *arg[], int argnum)
{
    if (arg[1] == NULL)
    {
        printf("Why would you want to print nothing? It's a print function.\n");
        setenv("EXITCODE", "EXIT_FAILURE", 1);
    }
    else
    {
        int check = 0;
        char *temp;
        if (strchr(arg[1],'\"') != NULL && strchr(arg[argnum-1],'\"') != NULL)
        {
            check = 1;
        }
        for (int i = 1; i < argnum; ++i)
        {
            if (strchr(arg[i], '$') != NULL && check == 0)
            {
                temp = strtok(arg[i], "$\"");
                printf("%s ", getenv(temp));
            }
            else
            {
                temp = strtok(arg[i], "\"");
                printf("%s ", temp);
            }
        }
        printf("\n");
        setenv("EXITCODE", "EXIT_SUCCESS", 1);
    }
}

// Change working directory.
void changeDir(char *arg[])
{
    if (arg[1] == NULL || arg[2] != NULL)
    {
        printf("Input directory as argument, without spaces.\n");
        setenv("EXITCODE", "EXIT_FAILURE", 1);
    }
    else
    {
        setenv("CWD", arg[1], 1);
        setenv("EXITCODE", "EXIT_SUCCESS", 1);
    }
}

// Prints all shell variables.
void printAll(char *arg[])
{
    if (arg[1] != NULL)
    {
        printf("Function requires no arguments.\n");
        setenv("EXITCODE", "EXIT_FAILURE", 1);
    }
    else
    {
        printf("PATH=%s\n", getenv("PATH"));
        printf("PROMPT=%s\n", getenv("PROMPT"));
        printf("SHELL=%s\n", getenv("SHELL"));
        printf("USER=%s\n", getenv("USER"));
        printf("HOME=%s\n", getenv("HOME"));
        printf("CWD=%s\n", getenv("CWD"));
        printf("TERMINAL=%s\n", getenv("TERMINAL"));
        setenv("EXITCODE", "EXIT_SUCCESS", 1);
    }
}

// Reads a file and executes commands.
void sourceComm(char *arg[])
{
    if (arg[1] != NULL && arg[2] == NULL) {
        FILE *fp = fopen(arg[1], "r");
        if (fp == NULL)
        {
            printf("File not found\n");
            setenv("EXITCODE", "EXIT_FAILURE", 1);
            return;
        }
        else
        {
            char line[256],
                    *args[256],
                    *token;
            while (fgets(line, sizeof(line), fp)) {
                token = strtok(line, " ");
                for (tokenindex2 = 0; token != NULL && tokenindex2 < 255; ++tokenindex2) {
                    args[tokenindex2] = token;
                    token = strtok(NULL, " ");
                }

                args[tokenindex2] = NULL;

                if (args[0] != NULL) {
                    if (strchr(args[0], '=') != NULL) {
                        char *temp = strtok(args[0], "="),
                                *temp2 = strtok(NULL, "=");
                        if (strchr(temp2, '$') != NULL) {
                            char *temp3 = getenv(strtok(temp2, "$"));
                            setenv(temp, temp3, 1);
                        } else {
                            setenv(temp, temp2, 1);
                        }
                    } else {
                        inComm(args, tokenindex2);
                    }
                }
            }
        }
        fclose(fp);
        setenv("EXITCODE", "EXIT_SUCCESS", 1);
    }
    else
    {
        printf("Command takes one file name as argument");
        setenv("EXITCODE", "EXIT_FAILURE", 1);
    }
}
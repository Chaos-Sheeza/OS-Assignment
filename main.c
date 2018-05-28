#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include "linenoise-master/linenoise.h"
#include "InterComm.h"
#include "ExterComm.h"
#include "redir.h"

#define MAX_ARGS 255


char    *line,
        *token = NULL,
        *arg[MAX_ARGS];

int tokenIndex;

// Basic main with arguments: find out what arguments will be used for.
int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    char    path[255],
            pidpath[BUFSIZ];
    readlink("/proc/self/exe", pidpath, BUFSIZ);
    // getcwd() returns the current working directory.
    getcwd(path, sizeof(path));
    setenv("PROMPT","Enter a Command Fool: ", 1);
    setenv("CWD", path, 1);
    // Wanted to create a function that determines the binary location of the eggshell.
    setenv("SHELL", pidpath, 1);
    setenv("TERMINAL", ttyname(STDIN_FILENO), 1);
    // Unclear what to output.
    setenv("EXITCODE", "not set", 0);

    // Infinite loop that stops when exited or error in variable line.
    while ((line = linenoise(getenv("PROMPT"))) != NULL)
    {
        // Sets token = first word in the line after the prompt, and separates it from the rest of the line.
        token = strtok(line, " ");

        // Loops until maximum amount of arguments is reached or until no more arguments are found, and stores the
        // amount of loops/arguments.
        for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS-1; tokenIndex++)
        {
            // Sets first argument equal to the word taken before, then keeps tokenizing the line putting them in the
            // array arg.
            arg[tokenIndex] = token;
            token = strtok(NULL, " ");
        }

        // Setting the last argument as null.
        arg[tokenIndex] = NULL;

        if (arg[0] != NULL)
        {
/*
            if(strchr(line, '>') == 0)
            {
                out1(arg, tokenIndex);
            }
            else
          */if (strchr(arg[0], '=') != NULL) {
                char *temp = strtok(arg[0], "="),
                        *temp2 = strtok(NULL, "=");
                if (strchr(temp2, '$') != NULL) {
                    char *temp3 = getenv(strtok(temp2, "$"));
                    setenv(temp, temp3, 1);
                } else {
                    setenv(temp, temp2, 1);
                }
            }
            else if (strcmp(arg[0], "print") == 0 || strcmp(arg[0], "exit") == 0 || strcmp(arg[0], "source") == 0 || strcmp(arg[0], "chdir") == 0 || strcmp(arg[0], "all") == 0)
            {
                inComm(arg, tokenIndex);
            }
            else
            {
                exComm(arg);
            }
        }

        // Free allocated memory used by variable line.
        linenoiseFree(line);
    }
    return 0;
}

// Function to find shell name.
void setpath(void)
{

}
#include <stdlib.h>
#include <stdio.h>

void out1(char *args[], int argnum)
{
    int     i = 0;
    char    *temp[argnum],
            *tmp2;
    for(i = 0; args[i] != ">" ; i++){
        temp[i] = args[i];
    }
    i++;
    temp[i] = NULL;
    tmp2 = args[i];

    FILE *fp = fopen(tmp2, "w");


}
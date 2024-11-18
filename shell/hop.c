#include "hop.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void change_directory(char *newpath, char *home, char *prev_dir, bool *changed)
{
    char curr_dir[1000];
    if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
    {
        perror("Unable to fetch current directory\n");
        return;
    }

    if (newpath[0] == '~')
    {
        char temp[1000];
        snprintf(temp, sizeof(temp), "%s%s", home, newpath + 1);
        newpath = strdup(temp);
    }

    if (strcmp(newpath, "-") == 0 && strlen(newpath) == 1)
    {
        newpath = strdup(prev_dir);
    }
    if (chdir(newpath) == -1)
    {
        perror("Unable to change the directory\n");
        free(newpath);
        return;
    }

    if (getcwd(curr_dir, sizeof(curr_dir)) != NULL)
    {
        printf(RED_COLOR "%s" RESET_COLOR "\n", curr_dir);
    }
    *changed = true;
    free(newpath);
}

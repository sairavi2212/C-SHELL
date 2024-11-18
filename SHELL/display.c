#include "display.h"
void display_directory(char *home_dir)
{
    char cwd[1000];
    char hostname[1000];
    char *username = getlogin();
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Unable to fetch current directory");
        return;
    }
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("Unable to fetch system name");
        return;
    }
    char display_dir[1000];

    if (strcmp(cwd, home_dir) == 0)
    {
        strcpy(display_dir, "~");
    }
    else if (strncmp(cwd, home_dir, strlen(home_dir)) == 0)
    {
        sprintf(display_dir, "~%s", cwd + strlen(home_dir));
    }
    else
    {
        strcpy(display_dir, cwd);
    }
    printf(YELLOW_COLOR "<%s@%s:%s> " RESET_COLOR, username, hostname, display_dir);
}

void printextra(char *input, char *home_dir, int timetaken)
{
    char cwd[1000];
    char hostname[1000];
    char *username = getlogin();
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Unable to fetch current directory");
        return;
    }
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("Unable to fetch system name");
        return;
    }
    char display_dir[1000];

    if (strcmp(cwd, home_dir) == 0)
    {
        strcpy(display_dir, "~");
    }
    else if (strncmp(cwd, home_dir, strlen(home_dir)) == 0)
    {
        sprintf(display_dir, "~%s", cwd + strlen(home_dir));
    }
    else
    {
        strcpy(display_dir, cwd);
    }
    printf(YELLOW_COLOR "<%s@%s:%s %s :%ds> " RESET_COLOR, username, hostname, display_dir, input, timetaken);
}


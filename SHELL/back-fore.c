#include "back-fore.h"
int getcommand(int pid)
{
    for (int i = 0; i < 4096; i++)
    {
        if (background_proc[i] != NULL && background_proc[i]->pid == pid)
        {
            return i;
        }
    }
    return -1;
}

void remove_process(int pid)
{
    for (int i = 0; i < 4096; i++)
    {
        if (background_proc[i] != NULL && background_proc[i]->pid == pid)
        {
            free(background_proc[i]);
            background_proc[i] = NULL;
            break;
        }
    }
}

void sigchld_handler(int num)
{
    int status;
    int pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        int ind = getcommand(pid);
        if (ind == -1)
        {
            continue;
        }
        char *s = strdup(background_proc[ind]->command);
        if (WIFEXITED(status))
        {
            remove_process(pid);
            printf("%s exited normally (%d) \n ", s, pid);
        }
        else if (WIFSIGNALED(status))
        {
            remove_process(pid);
            printf("%s exited abnormally by signal %d (%d)\n", s, WTERMSIG(status), pid);
        }
        else if (WIFSTOPPED(status))
        {
            printf("%s stopped by signal %d (%d)\n", s, WSTOPSIG(status), pid);
        }
    }
}

void store_background(char *command, int rc)
{
    while (cnt < 4096 && background_proc[cnt] != NULL){
        cnt++;
    }
    if (cnt == 4096)
    {
        int i = 0;
        while (i < 4096 && background_proc[i] != NULL)
            i++;
        cnt = i;
    }
    if (cnt == 4096)
    {
        printf("Maximum background processes reached\n");
        return;
    }
    background_proc[cnt] = (bg_commands *)malloc(sizeof(bg_commands));
    if (!background_proc[cnt])
    {
        perror("Failed to allocate memory for background process");
        exit(EXIT_FAILURE);
    }
    background_proc[cnt]->pid = rc;
    background_proc[cnt]->command = strdup(command);
    if (!background_proc[cnt]->command)
    {
        perror("Failed to duplicate command string");
        free(background_proc[cnt]);
        exit(EXIT_FAILURE);
    }
    cnt++;
}

char *get_foreground(char *command)
{
    pair *refined = remove_spaces(command);
    if (strcmp(refined->tokens[refined->num_commands - 1], "&") == 0)
        return NULL;
    return command;
}
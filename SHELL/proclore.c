#include "proclore.h"

void do_proclore(int id)
{
    char exe_stat[256];
    int pid;
    char status_file[256];
    char state[10];
    int groupid;
    char virtual_size[50];
    char *proc_status = NULL;
    char *vm_size = NULL;
    snprintf(status_file, sizeof(status_file), "/proc/%d/stat", id);
    FILE *file = fopen(status_file, "r");
    if (file == NULL)
    {
        printf("Invalid pid\n");
        return;
    }
    char tmp[256];
    for (int i = 1; i <= 52; i++)
    {
        fscanf(file, "%s", tmp);
        if (i == 1)
        {
            pid = atoi(tmp);
        }
        else if (i == 3)
        {
            strcpy(state, tmp);
        }
        else if (i == 5)
        {
            groupid = atoi(tmp);
        }
        else if (i == 23)
        {
            strcpy(virtual_size, tmp);
        }
    }
    printf("PID: %d\n", pid);
    if (pid == getpgrp())
    {
        printf("Process Status: %s+\n", state);
    }
    else
    {
        printf("Process Status: %s\n", state);
    }
    printf("Process group: %d\n", groupid);
    printf("Virtual Memory: %s\n", virtual_size);
    fclose(file);
    snprintf(exe_stat, sizeof(exe_stat), "/proc/%d/exe", id);
    char exe_path[256];
    ssize_t nbytes = readlink(exe_stat, exe_path, 256);
    if (nbytes != -1)
    {
        exe_path[nbytes] = '\0';
    }
    else
    {
        perror("readlink error\n");
    }
    printf("Executable path: %s\n", exe_path);
}
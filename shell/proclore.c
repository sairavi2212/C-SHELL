#include "proclore.h"


void do_proclore(int id)
{
    char exe_stat[256];
    char pid[256];
    char status_file[256];
    char state[10];
    char groupid[50];
    char foregroupid[50];
    char virtual_size[50];
    char *proc_status = NULL;
    char *vm_size = NULL;
    char *process_grp = NULL;
    snprintf(status_file, sizeof(status_file), "/proc/%d/stat", id);
    FILE *file = fopen(status_file, "r");
    if(file==NULL){
        printf("Invalid pid\n");
        return;
    }
    char tmp[256];
    for (int i = 1; i <= 52; i++)
    {
        fscanf(file, "%s", tmp);
        if (i == 1)
        {
            strcpy(pid, tmp);
        }
        else if (i == 3)
        {
            strcpy(state, tmp);
        }
        else if (i == 5)
        {
            strcpy(groupid, tmp);
        }
        else if (i == 23)
        {
            strcpy(virtual_size, tmp);
        }
        else if (i == 8)
        {
            strcpy(foregroupid, tmp);
        }
    }
    printf("PID: %s\n", pid);
    if (strcmp(foregroupid, groupid) == 0)
    {
        printf("Process Status: %s+\n", state);
    }
    else
    {
        printf("Process Status: %s\n", state);
    }
    printf("Process group: %s\n", groupid);
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
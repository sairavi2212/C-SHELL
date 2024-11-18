#include "header.h"
#include "activities.h"

int compare_bg_commands(const void *a, const void *b)
{
    bg_commands *cmd_a = *(bg_commands **)a;
    bg_commands *cmd_b = *(bg_commands **)b;
    return strcmp(cmd_a->command, cmd_b->command);
}

void print_activities()
{
    bg_commands *active_procs[4096];
    int count = 0;
    for (int i = 0; i < 4096; i++)
    {
        if (background_proc[i] != NULL)
        {
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%d/stat", background_proc[i]->pid);
            FILE *file = fopen(path, "r");
            if (file == NULL)
            {
                printf("Incorrect PID:%d\n", background_proc[i]->pid);
                return;
            }
            int pid;
            char state;
            char cmd[100];
            fscanf(file, "%d %s %c", &pid, cmd, &state);
            if (state == 'R' || state == 'Z' || state == 'S')
            {
                strcpy(background_proc[i]->state, "Running");
            }
            else
                strcpy(background_proc[i]->state, "Stopped");
            active_procs[count++] = background_proc[i];
            fclose(file);
        }
    }
    qsort(active_procs, count, sizeof(bg_commands *), compare_bg_commands);
    for (int i = 0; i < count; i++)
    {
        printf("[%d] : %s - %s\n", active_procs[i]->pid, active_procs[i]->command, active_procs[i]->state);
    }
}
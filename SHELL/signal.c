#include "signal.h"
#include "back-fore.h"

void ping(char *command)
{
    pair *refined = different_commands(command, " ");
    if (refined->num_commands != 3)
    {
        printf("Invalid syntax\n");
        return;
    }
    int pid = atoi(refined->tokens[1]);
    int signal_number = atoi(refined->tokens[2]);
    signal_number %= 32;
    kill(pid, signal_number);
}

void handle_ctrl_d()
{
    printf("\nLogging out of shell ....\n");
    for (int i = 0; i < 4096; i++)
    {
        if (background_proc[i] != NULL)
        {
            kill(background_proc[i]->pid, SIGKILL);
        }
    }
}

void handle_sigint(int sig)
{
    if (foreground_id > 0)
    {
        kill(foreground_id, SIGINT);
        foreground_id = -1;
        printf("\n");
    }
    else
    {
        printf("\nNo process is running in the foreground\n");
        return;
    }
}

char *getproc_withpid(int pid)
{
    char path[1024];
    snprintf(path, 1024, "/proc/%d/comm", pid);
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return NULL;
    }
    char *process = (char *)malloc(256 * sizeof(char));
    if (fgets(process, sizeof(process), file) != NULL)
    {
        process[strcspn(process, "\n")] = '\0';
        fclose(file);
        return process;
    }
    else
    {
        free(process);
        fclose(file);
        return NULL;
    }
}

void handle_sigtstp(int sig)
{
    if (foreground_id > 0)
    {
        kill(foreground_id, SIGTSTP);
        char *cmd = getproc_withpid(foreground_id);
        if (cmd == NULL)
        {
            printf("\nNo process is running in foreground\n");
            return;
        }
        else
        {
            store_background(cmd, foreground_id);
        }
        foreground_id = -1;
        printf("\n");
    }
    else
    {
        printf("\nNo process is running in foreground\n");
        return;
    }
}

void bring_to_foreground(int pid)
{
    if (kill(pid, 0) == -1)
    {
        printf("Process %d not found\n", pid);
        return;
    }
    if (kill(pid, SIGCONT) == -1)
    {
        printf("Could not begin the process %d\n", pid);
        return;
    }
    foreground_id = pid;
    waitpid(pid, NULL, WUNTRACED);
    remove_process(pid);
    foreground_id = -1;
}

void run_in_background(int pid)
{
    if (kill(pid, 0) == -1)
    {
        printf("Process %d\n not found\n", pid);
        return;
    }
    if (kill(pid, SIGCONT) == -1)
    {
        printf("Could not begin the process %d\n", pid);
        return;
    }
}

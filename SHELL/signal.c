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

// WNOHANG option is basically used when we have a child process and want to track it
// If we do normal waitpid on child process, then it would block the parent process and wait for child process to finish
// But if we use WNOHANG, then it will not block the parent process and will return immediately if the child process is still running
// It is like a non-blocking wait where we just get the status of the child process every second or so

/*
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        exit(1);
    }

    if (child == 0) {
        // Child: simulate a task that takes 5 seconds
        printf("[child] PID %d: starting work...\n", getpid());
        sleep(5);
        printf("[child] PID %d: work done, exiting.\n", getpid());
        exit(42);
    }
    else {
        // Parent
        int status;
        pid_t ret;

        // Loop until the child exits
        while (1) {
            ret = waitpid(child, &status, WNOHANG);
            if (ret == 0) {
                // Child still running
                printf("[parent] child not finished yet; doing other work...\n");
                sleep(1);
            }
            else if (ret == child) {
                // Child has exited or been reaped
                if (WIFEXITED(status)) {
                    printf("[parent] child exited with status %d\n",
                           WEXITSTATUS(status));
                }
                else {
                    printf("[parent] child terminated abnormally\n");
                }
                break;
            }
            else {
                // ret == -1: error (e.g., no such child)
                perror("waitpid");
                break;
            }
        }

        printf("[parent] all done, parent exiting.\n");
    }

    return 0;
}

*/



// WUNTRACED bit 
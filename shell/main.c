#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "display.h"
#include "input.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "header.h"
#include "proclore.h"
#include "seek.h"
#include <asm-generic/signal-defs.h>
#include <sys/time.h>
#define buff 4096
int takentime = 0;
int cnt = 0;

typedef struct bg_commands
{
    int pid;
    char *command;
} bg_commands;

struct timespec fstart, fend;
bg_commands *background_proc[4096];

char home_dir[1000];
char prev_dir[1000];
pair *pair_of_commands_and_count = NULL;
char **all_commands = NULL;
int number_of_commands = 0;

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

void sigchld_handler(int num)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        int ind = getcommand(pid);
        if (ind == -1)
        {
            printf("No command found for PID %d\n", pid);
            continue;
        }
        char *s = strdup(background_proc[ind]->command);
        if (WIFEXITED(status))
        {
            printf("%s exited normally (%d) \n ", s, pid);
        }
        else if (WIFSIGNALED(status))
        {
            printf("%s exited abnormally by signal %d (%d)\n", s, WTERMSIG(status), pid);
        }
        else if (WIFSTOPPED(status))
        {
            printf("%s stopped by signal %d (%d)\n", s, WSTOPSIG(status), pid);
        }
        free(s);
        free(background_proc[ind]);
        background_proc[ind] = NULL;
    }
}

void store_background(char *command, int rc)
{
    int cnt = -1;
    for (int i = 0; i < 4096; i++)
    {
        if (background_proc[i] == NULL)
        {
            cnt = i;
            break;
        }
    }
    if (cnt == -1)
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
}

char *get_foreground(char *command)
{
    pair *refined = remove_spaces(command);
    if (strcmp(refined->tokens[refined->num_commands - 1], "&") == 0)
        return NULL;
    return command;
}

void process(char *command, bool *log_presence)
{
    char *check = strstr(command, "&");
    if (check)
    {
        char *input_copy = strdup(command);
        pair *refinedamp = different_commands(command, "&");
        int i = 0;
        while (i < refinedamp->num_commands - 1)
        {
            char *comm = refinedamp->tokens[i];
            if (is_whitespace(comm))
            {
                continue;
            }
            pair *refined = remove_spaces(strdup(comm));
            char *myargs[15];
            for (int i = 0; i < refined->num_commands; i++)
            {
                myargs[i] = strdup(refined->tokens[i]);
            }
            myargs[refined->num_commands] = NULL;
            int rc = fork();
            if (rc < 0)
            {
                perror("fork failed\n");
                exit(0);
            }
            else if (rc == 0)
            {
                if (execvp(myargs[0], myargs) == -1)
                {
                    perror("Invalid command");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            printf("PID: %d\n", rc);
            store_background(myargs[0], rc);
            i++;
        }
        if (get_foreground(command))
        {
            process(refinedamp->tokens[refinedamp->num_commands - 1], false);
        }
        else
        {
            char *comm = refinedamp->tokens[i];
            if (is_whitespace(comm))
            {
                return;
            }
            pair *refined = remove_spaces(strdup(comm));
            char *myargs[15];
            for (int i = 0; i < refined->num_commands; i++)
            {
                myargs[i] = strdup(refined->tokens[i]);
            }
            myargs[refined->num_commands] = NULL;
            int rc = fork();
            if (rc < 0)
            {
                perror("fork failed\n");
                exit(0);
            }
            else if (rc == 0)
            {
                if (execvp(myargs[0], myargs) == -1)
                {
                    perror("Invalid command");
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            printf("PID: %d\n", rc);
            store_background(myargs[0], rc);
        }
    }
    else
    {
        pair *refined = remove_spaces(strdup(command));

        if (strcmp(refined->tokens[0], "hop") == 0)
        {
            char curr_dir[1000];
            if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
            {
                perror("Unable to fetch current directory\n");
                return;
            }
            if (refined->num_commands == 1)
            {
                bool changed = false;
                char *path = "~";
                change_directory(path, home_dir, prev_dir, &changed);
                if (changed)
                    strcpy(prev_dir, curr_dir);
            }
            for (int i = 1; i < refined->num_commands; i++)
            {
                bool changed = false;
                if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
                {
                    perror("Unable to fetch current directory\n");
                    return;
                }
                char *path = refined->tokens[i];
                change_directory(path, home_dir, prev_dir, &changed);
                if (changed)
                    strcpy(prev_dir, curr_dir);
            }
        }
        else if (strcmp(refined->tokens[0], "reveal") == 0)
        {
            details *dt = parse_command(refined->tokens, prev_dir, home_dir, refined->num_commands);
            print_all_details(dt->path, dt->a_flag, dt->l_flag);
        }
        else if (strcmp(refined->tokens[0], "log") == 0)
        {
            *log_presence = true;
            if (refined->num_commands == 1)
            {
                print_commands(home_dir);
            }
            else if (refined->num_commands == 2 && strcmp(refined->tokens[1], "purge") == 0)
            {
                log_purge(home_dir);
            }
            else if (refined->num_commands == 3 && strcmp(refined->tokens[1], "execute") == 0)
            {
                int index = atoi(refined->tokens[2]);
                log_execute(home_dir, index);
            }
            else
            {
                printf("Invalid Command\n");
            }
            return;
        }
        else if (strcmp(refined->tokens[0], "proclore") == 0)
        {
            int process_id;
            if (refined->num_commands == 1)
            {
                process_id = getpid();
            }
            else
            {
                process_id = atoi(refined->tokens[1]);
            }
            do_proclore(process_id);
        }
        else if (strcmp(refined->tokens[0], "seek") == 0)
        {
            char *prev = strdup(prev_dir);
            handle_seek_command(refined->tokens, refined->num_commands, home_dir, &prev);
            strcpy(prev_dir, prev);
        }
        else if (strcmp(refined->tokens[0], "exit") == 0)
        {
            for (int i = 0; i < cnt; i++)
            {
                free(background_proc[i]->command);
                free(background_proc[i]);
            }
            exit(EXIT_SUCCESS);
        }
        else
        {
            char *command = refined->tokens[0];
            int rc = fork();
            if (rc < 0)
            {
                perror("Failed to fork\n");
                exit(1);
            }
            if (rc == 0)
            {
                char *myargs[refined->num_commands];
                for (int i = 0; i < refined->num_commands; i++)
                {
                    myargs[i] = refined->tokens[i];
                }
                refined->tokens[refined->num_commands] = NULL;
                execvp(refined->tokens[0], refined->tokens);
                perror("Incorrect command\n");
                exit(1);
            }
            if (rc > 0)
            {
                waitpid(rc, NULL, 0);
            }
            return;
        }
    }
}

void go(char *input, bool done)
{
    if (checker(input))
    {
        log_command(input, home_dir);
        return;
    }
    pair *pair_of_commands_and_count = different_commands(input, ";");
    char **all_commands = pair_of_commands_and_count->tokens;
    int number_of_commands = pair_of_commands_and_count->num_commands;
    bool log_there = false;
    bool dontwrite = false;
    for (int i = 0; i < strlen(input) - 1; i++)
    {
        if (input[i] == ';' && input[i + 1] == ';')
        {
            printf("Invalid command\n");
            return;
        }
        if (input[i] == '&' && input[i + 1] == '&')
        {
            printf("Invalid command\n");
            return;
        }
    }
    for (int i = 0; i < number_of_commands; i++)
    {
        char *command = all_commands[i];
        if (is_whitespace(command))
        {
            continue;
        }
        process(command, &log_there);
        if (log_there)
        {
            dontwrite = true;
        }
    }
    if (done)
    {
        return;
    }
    if (!dontwrite)
    {
        log_command(input, home_dir);
    }
}

int main()
{
    for (int i = 0; i < 4096; i++)
    {
        background_proc[i] = NULL;
    }
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (getcwd(home_dir, sizeof(home_dir)) == NULL)
    {
        perror("Unable to fetch home directory");
        exit(1);
    }
    strcpy(prev_dir, home_dir);
    char input[4096];
    while (1)
    {
        if (takentime >= 2)
        {
            printextra(input, home_dir, takentime);
            takentime = 0;
        }
        else
            display_directory(home_dir);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        int start = time(NULL);
        go(input, false);
        int end = time(NULL);
        takentime = end - start;
    }
}

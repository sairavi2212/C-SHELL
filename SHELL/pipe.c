#include "header.h"
#include "pipe.h"
#include "redirection.h"

void execute_pipe(char *command)
{
    pair *refined = different_commands(command, "|");
    int cnt = 0;
    for (int i = 0; i < refined->num_commands; i++)
    {
        if (is_whitespace(refined->tokens[i]))
            continue;
        cnt++;
    }
    if (strstr(command, "||"))
    {
        printf("Invalid command\n");
        return;
    }

    int num_pipes = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '|')
            num_pipes++;
    }
    if (num_pipes + 1 != cnt)
    {
        printf("Invalid syntax\n");
        return;
    }
    int pipe_fd[2];
    int prev_fd = -1;
    int fd_input = dup(STDIN_FILENO);
    int fd_output = dup(STDOUT_FILENO);
    for (int i = 0; i < refined->num_commands; i++)
    {
        if (i < refined->num_commands - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("Unable to create pipe\n");
                return;
            }
        }

        int pid = fork();
        if (pid < 0)
        {
            perror("Unable to fork\n");
            return;
        }

        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) < 0)
                {
                    perror("Unable to duplicate stdin\n");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }

            if (i < refined->num_commands - 1)
            {
                if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
                {
                    perror("Unable to duplicate stdout\n");
                    exit(EXIT_FAILURE);
                }
                close(pipe_fd[0]);
            }
            else
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            go(refined->tokens[i]);
            exit(0);
        }
        else
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (prev_fd != -1)
            {
                close(prev_fd);
            }
            if (i < refined->num_commands - 1)
            {
                close(pipe_fd[1]);
            }
            prev_fd = pipe_fd[0];
        }
    }
    if (dup2(fd_input, STDIN_FILENO) < 0)
    {
        perror("Unable to restore stdin\n");
    }
    if (dup2(fd_output, STDOUT_FILENO) < 0)
    {
        perror("Unable to restore stdout\n");
    }

    close(fd_input);
    close(fd_output);
}

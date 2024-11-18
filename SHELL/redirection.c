#include "header.h"
#include "redirection.h"
bool is_redirect(char *command)
{
    if ((strchr(command, '<') == NULL) && (strchr(command, '>') == NULL) && (strstr(command, ">>") == NULL))
        return false;
    return true;
}

char *get_input_file(char *command)
{
    char *input_file = malloc(sizeof(char) * 100);
    int i = 0, j = 0;

    while (command[i] != '\0' && command[i] != '<')
        i++;

    if (command[i] == '\0')
    {
        free(input_file);
        return NULL; 
    }

    i++;
    while (isspace(command[i]))
        i++;

    while (command[i] != '\0' && !isspace(command[i]) && command[i] != '<' && command[i] != '>')
    {
        input_file[j++] = command[i++];
    }

    input_file[j] = '\0';
    return input_file;
}

char *get_output_file(char *command)
{
    char *output_file = malloc(sizeof(char) * 100);
    int i = 0, j = 0;

    while (command[i] != '\0' && command[i] != '>')
        i++;

    if (command[i] == '\0')
    {
        free(output_file);
        return NULL; 
    }

    if (command[i + 1] == '>')
        i += 2; 
    else
        i++;

    while (isspace(command[i]))
        i++;

    while (command[i] != '\0' && !isspace(command[i]) && command[i] != '<' && command[i] != '>')
    {
        output_file[j++] = command[i++];
    }

    output_file[j] = '\0';
    return output_file;
}

char *get_command(char *command)
{
    int i = 0;
    char *cmd = malloc(sizeof(char) * 100);
    int j = 0;

    while (isspace(command[i]))
        i++;

    while (command[i] != '\0' && !isspace(command[i]) && command[i] != '<' && command[i] != '>')
    {
        cmd[j++] = command[i++];
    }

    cmd[j] = '\0';
    return cmd;
}

void handle_redirection(char *command)
{
    char input_file[100] = {0};
    char input_exec[100] = {0};
    char output_exec[100] = {0};
    char output_file[100] = {0};
    bool append = false;

    char *input_red = strchr(command, '<');

    char *output_redir_append = strstr(command, ">>");
    char *output_redir_new = strchr(command, '>');
    pair *refined = different_commands(command, ">");
    if (input_red)
    {
        strcpy(input_exec, get_command(command));
        strcpy(input_file, get_input_file(command));
        if (output_redir_new)
        {
            strcpy(output_file, get_output_file(command));
        }
    }
    else if (output_redir_new)
    {
        strcpy(output_exec,refined->tokens[0]);
        strcpy(output_file, get_output_file(command));
    }
    if (output_redir_append)
        append = true;
    int fd = -1;
    int fd_output = dup(STDOUT_FILENO);

    int input_redirected;
    int fd_input = dup(STDIN_FILENO);
    if (input_red)
    {
        input_redirected = open(input_file, O_RDONLY);
        if (input_redirected == -1)
        {
            printf("No such input file found: %s\n", input_file);
            return;
        }
        dup2(input_redirected, STDIN_FILENO);
        close(input_redirected);

        if (output_redir_new)
        {
            if (append)
            {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else
            {
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd == -1)
            {
                printf("Unable to find file for output\n");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        go(input_exec);
    }

    else if (output_redir_new != NULL && input_red == NULL)
    {
        fd = -1;
        if (append)
        {
            fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (fd == -1)
        {
            printf("Unable to find file for output\n");
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        go(output_exec);
    }
    dup2(fd_input, STDIN_FILENO);
    close(fd_input);
    dup2(fd_output, STDOUT_FILENO);
    close(fd_output);
}
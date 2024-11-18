#include "input.h"
#include "header.h"
bool containback = false;

pair *different_commands(char *input, char *delimiter)
{
    char *input_copy = strdup(input);
    int count = 0;
    char **tokens = NULL;

    char *token = strtok(input_copy, delimiter);
    while (token)
    {
        tokens = realloc(tokens, (count + 1) * sizeof(char *));
        tokens[count++] = strdup(token);
        token = strtok(NULL, delimiter);
    }

    free(input_copy);
    pair *to_ret = malloc(sizeof(pair));
    to_ret->tokens = tokens;
    to_ret->num_commands = count;
    return to_ret;
}

pair *remove_spaces(char *command)
{
    return different_commands(command, " \t");
}

bool is_whitespace(char *command)
{
    for (int i = 0; i < strlen(command); i++)
    {
        if (!isspace(command[i]))
            return false;
    }
    return true;
}

int checker(char *input)
{
    int len = strlen(input);
    int i = 0;
    int flag = 0;
    for (int i = 0; i < len; i++)
    {
        if (flag == 0)
        {
            if (input[i] == '&')
            {
                flag = 1;
            }
        }
        else if (flag == 1)
        {
            if (input[i] == ';')
            {
                printf("Invalid Syntax\n");
                return 1;
            }
            if (input[i] == '&')
            {
                flag = 0;
                printf("Invalid Syntax\n");
                return 1;
            }
            else if (input[i] != ' ')
                flag = 0;
        }
    }
    return 0;
}

char *strip(char *cmd)
{
    char *cleaned_cmd = malloc(200 * sizeof(char));
    int ptr = 0;

    while (isspace(cmd[ptr]))
        ptr++;

    int rev_ptr = strlen(cmd) - 1;
    while (isspace(cmd[rev_ptr]))
        rev_ptr--;

    int j = 0;
    for (int i = ptr; i <= rev_ptr; i++)
        cleaned_cmd[j++] = cmd[i];

    cleaned_cmd[j] = '\0';
    return cleaned_cmd;
}

void handle_alias()
{
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open myshrc file\n");
        return;
    }

    char line[200];
    int cnt = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "//") || strlen(line) == 0)
            continue;

        char *tok = strtok(line, " ");
        if (strcmp(tok, "alias") == 0)
        {
            tok = strtok(NULL, "=");
            if (tok == NULL)
                continue;
            char *alias_name = strip(tok);

            tok = strtok(NULL, "=");
            if (tok == NULL)
                continue;
            char *alias_cmd = strip(tok);

            strcpy(all_aliases[cnt].alias, alias_name);
            strcpy(all_aliases[cnt].cmd, alias_cmd);
            cnt++;

            free(alias_name);
            free(alias_cmd);
        }
    }

    fclose(file);
}

char *check_alias_presence(char *command)
{
    bool found = false;
    char *new_command = malloc(sizeof(char) * 1096);
    strcpy(new_command, command);
    int ind = 0;
    while (isspace(*command))
    {
        command++;
    }
    for (int i = 0; i < 2; i++)
    {
        if (strncmp(command, all_aliases[i].alias, strlen(all_aliases[i].alias)) == 0)
        {
            strcpy(new_command, all_aliases[i].cmd);
            strcat(new_command, " ");
            strcat(new_command, command + strlen(all_aliases[i].alias));
            break;
        }
    }

    return new_command;
}
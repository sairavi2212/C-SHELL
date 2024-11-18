#include "input.h"
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
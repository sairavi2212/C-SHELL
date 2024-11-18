#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include<dirent.h>

typedef struct pair
{
    char **tokens;
    int num_commands;
} pair;

pair *different_commands(char *input, char *delimeter);

pair *remove_spaces(char *command);

bool is_whitespace(char* command);

int checker(char* input);
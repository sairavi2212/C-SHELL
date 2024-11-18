#ifndef INPUT_H
#define INPUT_H
#include "header.h"

pair *different_commands(char *input, char *delimeter);

pair *remove_spaces(char *command);

bool is_whitespace(char *command);

int checker(char *input);

void handle_alias();

char* check_alias_presence(char *command);

#endif
#ifndef REDIRECT_H
#define REDIRECT_H
#include "header.h"
#include "input.h"
bool is_redirect(char* command);

void handle_redirection(char* command);

#endif


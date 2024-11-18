#ifndef LOG_H
#define LOG_H
#include "header.h"

void log_command(char* command,char* home);
void log_purge(char* home);
void log_execute(char* home,int number);
void print_commands(char* home);
#endif
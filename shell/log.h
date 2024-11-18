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

void log_command(char* command,char* home);
void log_purge(char* home);
void log_execute(char* home,int number);
void print_commands(char* home);
#ifndef HEAD_H
#define HEAD_H

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
#include <asm-generic/signal-defs.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

typedef struct bg_commands
{
    int pid;
    char *command;
    char state[10];
} bg_commands;

typedef struct pair
{
    char **tokens;
    int num_commands;
} pair;

typedef struct aliases{
    char cmd[100];
    char alias[100];
}aliases;

extern aliases all_aliases[2];

extern bg_commands *background_proc[4096];

extern void go(char *input);

extern int cnt;
extern int foreground_id;


#endif

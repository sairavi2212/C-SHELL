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

#define RED_COLOR "\033[31m"
#define RESET_COLOR "\033[0m"

void change_directory(char *newpath, char *home,char* prev_dir,bool* changed);
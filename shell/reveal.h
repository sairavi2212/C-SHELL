#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <time.h>

#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"
#define WHITE_COLOR "\033[1;37m"

typedef struct details
{
    bool a_flag;
    bool l_flag;
    char *path;
} details;

details *parse_command(char **tokens, char *prev_dir, char *home, int number_of_tokens);

void print_all_details(char *path,bool a_flag,bool l_flag);
#ifndef HOP_H
#define HOP_H
#include"header.h"
#define RED_COLOR "\033[31m"
#define RESET_COLOR "\033[0m"

void change_directory(char *newpath, char *home,char* prev_dir,bool* changed);
#endif
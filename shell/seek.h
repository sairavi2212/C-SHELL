#ifndef SEEK_H
#define SEEK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void handle_seek_command(char **tokens, int num_tokens, char *home_dir, char **prev);    
void search_directory(const char *target, const char *directory, bool only_dirs, bool only_files, bool execute_flag, int *match_count, int *dir_count, int *file_count, char *matched_path,char* actual_directory);
bool has_execute_permission(const char *path);
bool has_read_permission(const char *path);
#endif 
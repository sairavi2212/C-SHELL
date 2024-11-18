#include "seek.h"
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"

void handle_seek_command(char **tokens, int num_tokens, char *home_dir, char **prev)
{
    if (num_tokens < 2)
    {
        printf("Invalid command format\n");
        return;
    }

    bool only_dirs = false;
    bool only_files = false;
    bool execute_flag = false;
    char *target = NULL;
    char *directory = ".";

    for (int i = 1; i < num_tokens; i++)
    {
        if (strcmp(tokens[i], "-d") == 0)
        {
            only_dirs = true;
        }
        else if (strcmp(tokens[i], "-f") == 0)
        {
            only_files = true;
        }
        else if (strcmp(tokens[i], "-e") == 0)
        {
            execute_flag = true;
        }
        else if (target == NULL)
        {
            target = tokens[i];
        }
        else
        {
            directory = tokens[i];
        }
    }

    if (only_dirs && only_files)
    {
        printf("Invalid flags!\n");
        return;
    }
    char resolved_directory[1024];
    if (directory[0] == '~')
    {
        snprintf(resolved_directory, 1024, "%s%s", home_dir, directory + 1);
    }
    else if (directory[0] == '-')
    {
        strcpy(resolved_directory, *prev);
    }
    else
    {
        strcpy(resolved_directory, directory);
    }
    int match_count = 0;
    int dir_count = 0;
    int file_count = 0;
    char matched_path[1024] = "";

    search_directory(target, resolved_directory, only_dirs, only_files, execute_flag, &match_count, &dir_count, &file_count, matched_path, resolved_directory);

    if (match_count == 0)
    {
        printf("No match found!\n");
    }
    else if (execute_flag)
    {
        if (dir_count == 1 && file_count == 0)
        {
            if (has_execute_permission(matched_path))
            {
                char *curr = getcwd(NULL, 0);
                if (chdir(matched_path) == -1)
                {
                    perror("Failed to change directory\n");
                }
                else
                {
                    free(*prev);
                    *prev = strdup(curr);
                }
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
        else if (file_count == 1 && dir_count == 0)
        {
            if (has_read_permission(matched_path))
            {
                FILE *file = fopen(matched_path, "r");
                if (file)
                {
                    char line[256];
                    while (fgets(line, sizeof(line), file))
                    {
                        printf("%s", line);
                    }
                    printf("\n");
                    fclose(file);
                }
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
    }
}

void search_directory(const char *target, const char *directory, bool only_dirs, bool only_files, bool execute_flag, int *match_count, int *dir_count, int *file_count, char *matched_path, char *actual_directory)
{
    DIR *dir;
    struct dirent *entry;
    char path[1024];

    if ((dir = opendir(directory)) == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        struct stat statbuf;
        if (stat(path, &statbuf) == -1)
        {
            perror("stat");
            continue;
        }

        if (strncmp(entry->d_name, target, strlen(target)) == 0)
        {
            if ((S_ISDIR(statbuf.st_mode) && !only_files) || (S_ISREG(statbuf.st_mode) && !only_dirs))
            {
                (*match_count)++;
                strcpy(matched_path, path);

                if (S_ISDIR(statbuf.st_mode))
                {
                    if (isatty(STDOUT_FILENO))
                        printf("./%s\n", path + strlen(actual_directory) + 1);
                    else
                        printf("./%s\n", path + strlen(actual_directory) + 1);
                    (*dir_count)++;
                }
                else if (S_ISREG(statbuf.st_mode))
                {
                    if (isatty(STDOUT_FILENO))
                        printf(GREEN_COLOR "./%s\n" RESET_COLOR, path + strlen(actual_directory) + 1);
                    else
                        printf( "./%s\n", path + strlen(actual_directory) + 1);
                    (*file_count)++;
                }
            }
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            search_directory(target, path, only_dirs, only_files, execute_flag, match_count, dir_count, file_count, matched_path, actual_directory);
        }
    }

    closedir(dir);
}

bool has_execute_permission(const char *path)
{
    return access(path, X_OK) == 0;
}

bool has_read_permission(const char *path)
{
    return access(path, R_OK) == 0;
}

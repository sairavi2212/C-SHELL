#include "reveal.h"
#define BLOCK_SIZE 512
details *parse_command(char **tokens, char *prev_dir, char *home, int number_of_tokens)
{
    bool a_flag = false;
    bool l_flag = false;
    bool got_path = false;
    char *path = NULL;

    for (int i = 1; i < number_of_tokens; i++)
    {
        char *part = tokens[i];
        if (strcmp(part, "-a") == 0)
        {
            a_flag = true;
            continue;
        }
        if (strcmp(part, "-l") == 0)
        {
            l_flag = true;
            continue;
        }
        if ((strcmp(part, "-al") == 0) || strcmp(part, "-la") == 0)
        {
            a_flag = l_flag = true;
            continue;
        }
        if (strlen(part) == 1 && part[0] == '-')
        {
            path = strdup(prev_dir);
            got_path = true;
            break;
        }
        if (part[0] == '~')
        {
            char temp[1000];
            snprintf(temp, sizeof(temp), "%s%s", home, part + 1);
            path = strdup(temp);
            got_path = true;
            break;
        }
        if (strlen(part) == 1 && part[0] == '.')
        {
            path = getcwd(NULL, 0);
            got_path = true;
            break;
        }
        if (part[0] != '-')
        {
            path = strdup(part);
            got_path = true;
            break;
        }
        if (part[0] == '-' && strlen(part) > 1)
        {
            for (int i = 1; i < strlen(part); i++)
            {
                if (part[i] == 'a')
                    a_flag = true;
                if (part[i] == 'l')
                    l_flag = true;
            }
        }
    }

    details *dt = malloc(sizeof(details));
    dt->a_flag = a_flag;
    dt->l_flag = l_flag;
    if (!got_path)
        dt->path = getcwd(NULL, 0);
    else
        dt->path = path;

    return dt;
}

int compare(const void *a, const void *b)
{
    const char **stra = (const char **)a;
    const char **strb = (const char **)b;
    return strcmp(*stra, *strb);
}

void print_all_details(char *path, bool a_flag, bool l_flag)
{
    struct stat check;
    if (stat(path, &check) == -1)
    {
        perror("stat");
        return;
    }

    if (S_ISREG(check.st_mode))
    {
        if (l_flag)
        {
            printf((S_ISDIR(check.st_mode)) ? "d" : "-");
            printf((check.st_mode & S_IRUSR) ? "r" : "-");
            printf((check.st_mode & S_IWUSR) ? "w" : "-");
            printf((check.st_mode & S_IXUSR) ? "x" : "-");
            printf((check.st_mode & S_IRGRP) ? "r" : "-");
            printf((check.st_mode & S_IWGRP) ? "w" : "-");
            printf((check.st_mode & S_IXGRP) ? "x" : "-");
            printf((check.st_mode & S_IROTH) ? "r" : "-");
            printf((check.st_mode & S_IWOTH) ? "w" : "-");
            printf((check.st_mode & S_IXOTH) ? "x" : "-");
            printf(" ");

            printf("%ld ", check.st_nlink);

            struct passwd *pw = getpwuid(check.st_uid);
            printf("%s ", (pw != NULL) ? pw->pw_name : "UNKNOWN");

            struct group *gr = getgrgid(check.st_gid);
            printf("%s ", (gr != NULL) ? gr->gr_name : "UNKNOWN");

            printf("%ld ", check.st_size);

            char time_buf[256];
            struct tm *tm_info = localtime(&check.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
            printf("%s ", time_buf);
        }
        if (check.st_mode & S_IXUSR)
        {
            printf(GREEN_COLOR "%s*" RESET_COLOR "\n", path);
        }
        else
        {
            printf(WHITE_COLOR "%s" RESET_COLOR "\n", path);
        }
        return;
    }
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Unable to open the directory");
        return;
    }

    struct dirent *entity;
    int total = 0;
    char **inside_dir = NULL;
    int i = 0;
    while ((entity = readdir(dir)) != NULL)
    {
        if (!a_flag && entity->d_name[0] == '.')
        {
            continue;
        }
        inside_dir = realloc(inside_dir, (i + 1) * sizeof(char *));
        if (inside_dir == NULL)
        {
            perror("Unable to allocate memory");
            return;
        }
        inside_dir[i] = strdup(entity->d_name);
        if (inside_dir[i] == NULL)
        {
            perror("Unable to allocate memory for directory name");
            return;
        }
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, inside_dir[i]);
        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1)
        {
            perror("stat");
            closedir(dir);
            return;
        }
        total += file_stat.st_blocks;
        i++;
    }

    qsort(inside_dir, i, sizeof(char *), compare);
    if (l_flag)
    {
        printf("Total %d\n", total / 2);
    }
    for (int j = 0; j < i; j++)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, inside_dir[j]);
        struct stat file_stat;
        if (stat(full_path, &file_stat) == 0)
        {
            if (l_flag)
            {
                printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
                printf(" ");

                printf("%ld ", file_stat.st_nlink);

                struct passwd *pw = getpwuid(file_stat.st_uid);
                printf("%s ", (pw != NULL) ? pw->pw_name : "UNKNOWN");

                struct group *gr = getgrgid(file_stat.st_gid);
                printf("%s ", (gr != NULL) ? gr->gr_name : "UNKNOWN");

                printf("%ld ", file_stat.st_size);

                char time_buf[256];
                struct tm *tm_info = localtime(&file_stat.st_mtime);
                strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
                printf("%s ", time_buf);
            }
            if (S_ISDIR(file_stat.st_mode))
            {
                printf(BLUE_COLOR "%s/" RESET_COLOR "\n", inside_dir[j]);
            }
            else if (file_stat.st_mode & S_IXUSR)
            {
                printf(GREEN_COLOR "%s*" RESET_COLOR "\n", inside_dir[j]);
            }
            else
            {
                printf(WHITE_COLOR "%s" RESET_COLOR "\n", inside_dir[j]);
            }
        }
        else
        {
            printf(WHITE_COLOR "%s" RESET_COLOR "\n", inside_dir[j]);
        }
        free(inside_dir[j]);
    }

    free(inside_dir);
    closedir(dir);
}
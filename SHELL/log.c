#include "log.h"
#define PATH_SIZE 1024
char *history[15];
char path[1024];
int ind = 0;
void log_command(char *command, char *home)
{
    snprintf(path, PATH_SIZE, "%s/history.txt", home);
    FILE *file = fopen(path, "r");
    if (file != NULL)
    {
        char check_command[256];
        ind = 0;
        while (fgets(check_command, sizeof(check_command), file))
        {
            if (ind < 15)
            {
                history[ind] = strdup(check_command);
                history[ind][strcspn(history[ind], "\n")] = '\0';
                ind++;
            }
        }
        fclose(file);
    }
    if (ind > 0 && strcmp(command, history[ind - 1]) == 0)
    {
        return;
    }
    if (ind < 15)
    {
        history[ind] = strdup(command);
        ind++;
        file = fopen(path, "a");
        if (file != NULL)
        {
            fprintf(file, "%s\n", command);
            fclose(file);
        }
        else
        {
            printf("Failed to open history.txt for appending.\n");
        }
    }
    else
    {
        free(history[0]);
        for (int i = 1; i < 15; i++)
        {
            history[i - 1] = history[i];
        }
        history[14] = strdup(command);

        file = fopen(path, "w");
        if (file != NULL)
        {
            for (int i = 0; i < 15; i++)
            {
                fprintf(file, "%s\n", history[i]);
            }
            fclose(file);
        }
        else
        {
            printf("Failed to open history.txt for writing.\n");
        }
    }
}

void print_commands(char *home)
{
    snprintf(path, sizeof(path), "%s/history.txt", home);
    FILE *file = fopen(path, "r");
    if (file != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
        }
        fclose(file);
    }
    else
    {
        printf("Failed to open history.txt for reading.\n");
    }
}

void log_purge(char *home)
{
    snprintf(path, sizeof(path), "%s/history.txt", home);
    for (int i = 0; i < ind; i++)
    {
        free(history[i]);
        history[i] = NULL;
    }
    ind = 0;
    FILE *file = fopen(path, "w");
    fclose(file);
}
void log_execute(char *home, int number)
{
    snprintf(path, sizeof(path), "%s/history.txt", home);
    FILE *file = fopen(path, "r");
    int i = 0;
    if (file != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';
            history[i++] = strdup(line);
        }
        fclose(file);
    }
    else
    {
        printf("Failed to open history.txt for reading.\n");
        return;
    }
    int start = i - number;
    int flag = 1;
    if (start >= 0 && number > 0)
    {
        go(history[start]);
    }
    else
    {
        printf("Error: Index is invalid\n");
    }
}

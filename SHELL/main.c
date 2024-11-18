#include "allfiles.h"
#define buff 4096
int takentime = 0;

char home_dir[1000];
char prev_dir[1000];
pair *pair_of_commands_and_count = NULL;
char **all_commands = NULL;
int number_of_commands = 0;
aliases all_aliases[2];
bg_commands *background_proc[4096] = {NULL};
int foreground_id = -1;
int cnt = 0;
bool done = false;
void process(char *command, bool background)
{
    char *check = strstr(command, "&");
    if (check)
    {
        char *input_copy = strdup(command);
        pair *refinedamp = different_commands(command, "&");
        int i = 0;
        while (i < refinedamp->num_commands - 1)
        {
            char *comm = refinedamp->tokens[i];
            if (is_whitespace(comm))
            {
                continue;
            }
            pair *refined = remove_spaces(strdup(comm));
            process(comm, true);
            i++;
        }
        if (get_foreground(command))
        {
            process(refinedamp->tokens[refinedamp->num_commands - 1], false);
        }
        else
        {
            char *comm = refinedamp->tokens[i];
            if (is_whitespace(comm))
            {
                return;
            }
            pair *refined = remove_spaces(strdup(comm));
            process(comm, true);
        }
    }
    else
    {
        pair *refined = remove_spaces(strdup(command));
        if (strstr(command, "|"))
        {
            if (!background)
            {
                execute_pipe(command);
            }
            else
            {
                int rc = fork();
                if (rc < 0)
                {
                    perror("unable to fork\n");
                    return;
                }
                else if (rc == 0)
                {
                    setpgid(0, 0);
                    execute_pipe(command);
                }
                else
                {
                    printf("PID: %d\n", rc);
                    store_background(refined->tokens[0], rc);
                }
            }
        }
        else if (is_redirect(command))
        {
            if (!background)
            {
                handle_redirection(command);
            }
            else
            {
                int rc = fork();
                if (rc < 0)
                {
                    perror("unable to fork\n");
                    return;
                }
                else if (rc == 0)
                {
                    setpgid(0, 0);
                    handle_redirection(command);
                }
                else
                {
                    printf("PID: %d\n", rc);
                    store_background(refined->tokens[0], rc);
                }
            }
        }
        else if (strcmp(refined->tokens[0], "hop") == 0)
        {
            char curr_dir[1000];
            if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
            {
                perror("Unable to fetch current directory\n");
                return;
            }
            if (refined->num_commands == 1)
            {
                bool changed = false;
                char *path = "~";
                change_directory(path, home_dir, prev_dir, &changed);

                if (changed)
                    strcpy(prev_dir, curr_dir);
            }
            for (int i = 1; i < refined->num_commands; i++)
            {
                bool changed = false;
                if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
                {
                    perror("Unable to fetch current directory\n");
                    return;
                }
                char *path = refined->tokens[i];
                change_directory(path, home_dir, prev_dir, &changed);

                if (changed)
                    strcpy(prev_dir, curr_dir);
            }
        }
        else if (strcmp(refined->tokens[0], "reveal") == 0)
        {
            details *dt = parse_command(refined->tokens, prev_dir, home_dir, refined->num_commands);
            if (!background)
                print_all_details(dt->path, dt->a_flag, dt->l_flag);
            else
            {
                int rc = fork();
                if (rc < 0)
                {
                    perror("unable to fork\n");
                    return;
                }
                else if (rc == 0)
                {
                    setpgid(0, 0);
                    print_all_details(dt->path, dt->a_flag, dt->l_flag);
                }
                else
                {
                    printf("PID: %d\n", rc);
                    store_background(refined->tokens[0], rc);
                }
            }
        }
        else if (strcmp(refined->tokens[0], "log") == 0)
        {
            if (refined->num_commands == 1)
            {
                print_commands(home_dir);
            }
            else if (refined->num_commands == 2 && strcmp(refined->tokens[1], "purge") == 0)
            {
                log_purge(home_dir);
            }
            else if (refined->num_commands == 3 && strcmp(refined->tokens[1], "execute") == 0)
            {
                int index = atoi(refined->tokens[2]);
                log_execute(home_dir, index);
            }
            else
            {
                printf("Invalid Command\n");
            }
        }
        else if (strcmp(refined->tokens[0], "proclore") == 0)
        {
            int process_id;
            if (refined->num_commands == 1)
            {
                process_id = getpid();
            }
            else
            {
                process_id = atoi(refined->tokens[1]);
            }
            if (!background)
            {
                do_proclore(process_id);
            }
            else
            {
                int rc = fork();
                if (rc < 0)
                {
                    perror("unable to fork\n");
                    return;
                }
                else if (rc == 0)
                {
                    setpgid(0, 0);
                    do_proclore(process_id);
                }
                else
                {
                    printf("PID: %d\n", rc);
                    store_background(refined->tokens[0], rc);
                }
            }
        }
        else if (strcmp(refined->tokens[0], "seek") == 0)
        {
            char *prev = strdup(prev_dir);
            handle_seek_command(refined->tokens, refined->num_commands, home_dir, &prev);
            strcpy(prev_dir, prev);
        }
        else if (strcmp(refined->tokens[0], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(refined->tokens[0], "activities") == 0)
        {
            print_activities();
        }
        else if (strcmp(refined->tokens[0], "ping") == 0)
        {
            ping(command);
        }
        else if (strcmp(refined->tokens[0], "fg") == 0)
        {
            if (refined->num_commands != 2)
            {
                printf("Incorrect syntax\n");
                return;
            }
            else
            {
                int pid = atoi(refined->tokens[1]);
                bring_to_foreground(pid);
            }
        }
        else if (strcmp(refined->tokens[0], "bg") == 0)
        {
            if (refined->num_commands != 2)
            {
                printf("Incorrect syntax\n");
                return;
            }
            else
            {
                int pid = atoi(refined->tokens[1]);
                run_in_background(pid);
            }
        }
        else if (strcmp(refined->tokens[0], "neonate") == 0)
        {
            if (refined->num_commands != 3)
            {
                printf("Invalid Syntax\n");
                return;
            }
            else if (strcmp(refined->tokens[1], "-n") != 0)
            {
                printf("Invalid Syntax\n");
                return;
            }
            else
            {
                print_neonate(atoi(refined->tokens[2]));
            }
        }
        else if (strcmp(refined->tokens[0], "iMan") == 0)
        {
            if (refined->num_commands != 2)
            {
                printf("Invalid Syntax\n");
                return;
            }
            iman(refined->tokens[1]);
        }
        else
        {
            char *command = refined->tokens[0];
            int rc = fork();
            if (rc < 0)
            {
                perror("Failed to fork\n");
                exit(1);
            }
            if (rc == 0)
            {
                if ((strcmp(refined->tokens[0], "vim") != 0) && (strcmp(refined->tokens[0], "man") != 0) && (strcmp(refined->tokens[0], "vi") != 0))
                    setpgid(0, 0);
                char *myargs[refined->num_commands];
                for (int i = 0; i < refined->num_commands; i++)
                {
                    myargs[i] = refined->tokens[i];
                }
                refined->tokens[refined->num_commands] = NULL;
                execvp(refined->tokens[0], refined->tokens);
                perror("Incorrect command \n");
                exit(1);
            }
            if (rc > 0)
            {
                if (!background)
                {
                    foreground_id = rc;
                    waitpid(rc, NULL, WUNTRACED);
                }
                else
                {
                    printf("PID: %d\n", rc);
                    store_background(refined->tokens[0], rc);
                }
            }
        }
    }
}

void go(char *input)
{
    if (checker(input))
    {
        log_command(input, home_dir);
        return;
    }
    pair *pair_of_commands_and_count = different_commands(input, ";");
    char **all_commands = pair_of_commands_and_count->tokens;
    int number_of_commands = pair_of_commands_and_count->num_commands;
    bool log_there = false;
    bool dontwrite = false;
    for (int i = 0; i < strlen(input) - 1; i++)
    {
        if (input[i] == ';' && input[i + 1] == ';')
        {
            printf("Invalid command\n");
            return;
        }
        if (input[i] == '&' && input[i + 1] == '&')
        {
            printf("Invalid command\n");
            return;
        }
    }
    if (!done)
    {
        if (strstr(input, "log") == NULL)
        {
            log_command(input, home_dir);
        }
        done = true;
    }
    for (int i = 0; i < number_of_commands; i++)
    {
        char *command = all_commands[i];
        if (is_whitespace(command))
        {
            continue;
        }
        command = check_alias_presence(command);
        process(command, false);
    }
}

int main()
{
    for (int i = 0; i < 4096; i++)
    {
        background_proc[i] = NULL;
    }
    handle_alias();
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (getcwd(home_dir, sizeof(home_dir)) == NULL)
    {
        perror("Unable to fetch home directory");
        exit(1);
    }
    strcpy(prev_dir, home_dir);
    char input[4096];
    while (1)
    {
        done = false;
        if (takentime >= 2)
        {
            printextra(input, home_dir, takentime);
            takentime = 0;
        }
        else
            display_directory(home_dir);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            handle_ctrl_d();
            exit(EXIT_SUCCESS);
        }
        input[strcspn(input, "\n")] = '\0';
        int start = time(NULL);
        go(input);
        int end = time(NULL);
        takentime = end - start;
    }
}

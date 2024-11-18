#include "neonate.h"

pid_t get_recent_pid()
{
    FILE *file = fopen("/proc/loadavg", "r");
    if (!file)
    {
        perror("Unable to open file\n");
        return -1;
    }

    pid_t recent_pid;
    float a, b, c;
    int d, e;
    if (fscanf(file, "%f %f %f %d/%d %d", &a, &b, &c, &d, &e, &recent_pid) != 6)
    {
        perror("fscanf error\n");
        fclose(file);
        return -1;
    }
    fclose(file);
    return recent_pid;
}

void print_neonate(int time_arg)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    fd_set read_fds;
    struct timeval timeout;
    char c;
    while (1)
    {
        pid_t recent_pid = get_recent_pid();
        printf("%d\n", recent_pid);
        timeout.tv_sec = time_arg;
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        int result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
        if (result == -1)
        {
            perror("select error\n");
            break;
        }
        else if (result > 0)
        {
            if (FD_ISSET(STDIN_FILENO, &read_fds))
            {
                read(STDIN_FILENO, &c, 1);
                if (c == 'x' || c == 'X')
                {
                    break;
                }
            }
        }
    }
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
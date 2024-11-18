#include "iman.h"
#include "header.h"

#define BUFFER_SIZE 11000
#define PORT 80
char *source = "man.he.net";
char *page_source = "/?topic=%s&section=all";

void iman(char *command)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char page[100];

    snprintf(page, sizeof(page), page_source, command);

    host = gethostbyname(source);
    if (host == NULL)
    {
        printf("Error: Unknown host\n");
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting");
        close(sockfd);
        exit(1);
    }

    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             page, source);

    if (send(sockfd, request, strlen(request), 0) < 0)
    {
        perror("Error sending request");
        close(sockfd);
        exit(1);
    }

    ssize_t received;
    int in_tag = 0;
    bool done = false;
    while ((received = recv(sockfd, response, BUFFER_SIZE - 1, 0)) > 0)
    {
        response[received] = '\0';
        char *ptr = strstr(response, "<");
        while (*ptr)
        {
            if (in_tag)
            {
                if (*ptr == '>')
                {
                    in_tag = 0;
                }
            }
            else
            {
                if (*ptr == '<')
                {
                    in_tag = 1;
                }
                else
                {
                    printf("%c", *ptr);
                }
            }
            ptr++;
        }
    }

    if (received < 0)
    {
        perror("Error receiving response");
    }

    close(sockfd);
}

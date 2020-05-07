#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#define PORT 8081


typedef struct 
{
    int socket;
} context_t;

int main(int argc, char* argv[]) {

    context_t *context = malloc(sizeof(context_t));

// init co
    struct sockaddr_in address;
    context->socket = 0;
    int valread;
    struct sockaddr_in serv_addr;
    if ((context->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }

    if (connect(context->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
    }
    // Co inited

    char* buffer = malloc(sizeof(char)*256);

    while (1)
    {
        if (read( context->socket , buffer, 256)) printf("%s",buffer ); 
        memset(buffer, '\0', 256);
    }
}
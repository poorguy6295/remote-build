#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <array>

void error(char *msg) {
    perror(msg);
    exit(-1);
}

const int CLIENT_PORT = 3001;
const int SERVER_PORT = 3000;

int main(int argc, char const *argv[]) {
    struct sockaddr_in address, serv_addr;
    int sock = 0, valread;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error opening socket");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        error("Invalid address/ Address not supported ");
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    char *msg = "ls -la";
    send(sock, msg, strlen(msg), 0);
    valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );

    close(sock);

    return 0;
}
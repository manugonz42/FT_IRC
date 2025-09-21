#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }
    std::cout << "Listening on port " << port << std::endl;
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(server_fd);
        return 1;
    }
    char buffer[1024];
    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
    close(client_fd);
    close(server_fd);
    return 0;
}

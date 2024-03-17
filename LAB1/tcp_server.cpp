#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>

int main()
{
    struct sockaddr_in local;
    int server_sockfd;
    int client_sockfd;

    local.sin_family = AF_INET;
    local.sin_port = htons(8080);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sockfd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((client_sockfd = accept(server_sockfd, NULL, NULL)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    bool bool_in[3];
    long long_in[3];
    char server_response[] = "OK";

    auto start = std::chrono::high_resolution_clock::now();
    if (recv(client_sockfd, bool_in, sizeof(bool_in), 0) <= 0)
    {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int64_t mcs = elapsed.count();

    printf("%ld Received boolean: %d %d %d\n", mcs, bool_in[0], bool_in[1], bool_in[2]);

    start = std::chrono::high_resolution_clock::now();
    if (send(client_sockfd, server_response, sizeof(server_response), 0) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Send OK\n", mcs);

    start = std::chrono::high_resolution_clock::now();
    if (recv(client_sockfd, long_in, sizeof(long_in), 0) <= 0)
    {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Received long: %ld %ld %ld\n", mcs, long_in[0], long_in[1], long_in[2]);

    start = std::chrono::high_resolution_clock::now();
    if (send(client_sockfd, server_response, sizeof(server_response), 0) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Send OK\n", mcs);

    close(client_sockfd);
    close(server_sockfd);

    return 0;
}
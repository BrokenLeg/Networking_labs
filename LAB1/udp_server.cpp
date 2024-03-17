#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char server_response[] = "OK";

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    bool bool_in[3];
    long long_in[3];

    auto start = std::chrono::high_resolution_clock::now();
    int bytes_received = recvfrom(sockfd, bool_in, sizeof(bool_in), 0, (struct sockaddr *)&client_addr, &client_len);
    if (bytes_received < 0)
    {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int64_t mcs = elapsed.count();

    printf("%ld Recv boolean: %d %d %d\n", mcs, bool_in[0], bool_in[1], bool_in[2]);

    start = std::chrono::high_resolution_clock::now();
    if (sendto(sockfd, server_response, strlen(server_response), 0, (const struct sockaddr *)&client_addr, client_len) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Send OK\n", mcs);

    start = std::chrono::high_resolution_clock::now();
    bytes_received = recvfrom(sockfd, long_in, sizeof(long_in), 0, (struct sockaddr *)&client_addr, &client_len);
    if (bytes_received < 0)
    {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Recv long: %ld %ld %ld\n", mcs, long_in[0], long_in[1], long_in[2]);

    start = std::chrono::high_resolution_clock::now();
    if (sendto(sockfd, server_response, strlen(server_response), 0, (const struct sockaddr *)&client_addr, client_len) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Send OK\n", mcs);

    close(sockfd);

    return 0;
}

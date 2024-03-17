#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>

int main()
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    bool bool_msg[3] = {true, false, true};
    long long_msg[3];
    char server_response[100];

    auto start = std::chrono::high_resolution_clock::now();
    if (send(sockfd, bool_msg, sizeof(bool_msg), 0) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int64_t mcs = elapsed.count();

    printf("%ld Send boolean message\n", mcs);

    start = std::chrono::high_resolution_clock::now();
    if (recv(sockfd, server_response, 100, 0) <= 0)
    {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Received server response : %s\n", mcs, server_response);

    start = std::chrono::high_resolution_clock::now();
    if (send(sockfd, long_msg, sizeof(long_msg), 0) <= 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Send long message\n", mcs);

    start = std::chrono::high_resolution_clock::now();
    if (recv(sockfd, server_response, 100, 0) <= 0)
    {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    mcs = elapsed.count();

    printf("%ld Received server response : %s\n", mcs, server_response);

    close(sockfd);

    return 0;
}

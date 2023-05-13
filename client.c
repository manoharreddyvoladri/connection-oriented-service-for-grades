#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define MAX_SUBJECTS 5

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int marks[MAX_SUBJECTS];
    char grade;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Enter the marks for %d subjects:\n", MAX_SUBJECTS);
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        scanf("%d", &marks[i]);
    }

    if (send(sock, marks, sizeof(marks), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    if (recv(sock, &grade, sizeof(grade), 0) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    printf("Grade: %c\n", grade);

    close(sock);

    return 0;
}

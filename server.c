#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define MAX_SUBJECTS 5

void calculate_grade(int *marks, char *grade) {
    int total_marks = 0;
    float percentage;

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total_marks += marks[i];
    }

    percentage = ((float)total_marks / (MAX_SUBJECTS * 100)) * 100;

    if (percentage >= 90) {
        *grade = 'S';
    } else if (percentage >= 80 && percentage < 90) {
        *grade = 'A';
    } else if (percentage >= 70 && percentage < 80) {
        *grade = 'B';
    } else if (percentage >= 60 && percentage < 70) {
        *grade = 'C';
    } else if (percentage >= 50 && percentage < 60) {
        *grade = 'D';
    } else {
        *grade = 'E';
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int marks[MAX_SUBJECTS];
    char grade;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the PORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connection established with client\n");

    while (1) {
        
        if (recv(new_socket, marks, sizeof(marks), 0) == -1) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        calculate_grade(marks, &grade);

        if (send(new_socket, &grade, sizeof(grade), 0) == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }

        printf("Grade sent to client\n");
    }

    close(new_socket);
    close(server_fd);

    return 0;
}

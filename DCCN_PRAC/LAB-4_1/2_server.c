#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void count_even_odd(int *numbers, int size, int *even_count, int *odd_count) {
    *even_count = 0;
    *odd_count = 0;
    for (int i = 0; i < size; i++) {
        if (numbers[i] % 2 == 0)
            (*even_count)++;
        else
            (*odd_count)++;
    }
}

int main() {
    int server_fd, new_socket;

    struct sockaddr_in address;
    
    int addrlen = sizeof(address);
    int buffer[1024] = {0};
    int even_count, odd_count;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the network address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    // Accept the incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read the numbers from the client
    int valread = read(new_socket, buffer, 1024);
    int size = valread / sizeof(int);

    // Count even and odd numbers
    count_even_odd(buffer, size, &even_count, &odd_count);

    // Send the result back to the client
    int result[2] = {even_count, odd_count};
    send(new_socket, result, sizeof(result), 0);
    printf("Sent even_count = %d, odd_count = %d to client\n", even_count, odd_count);

    close(new_socket);
    close(server_fd);
    return 0;
}

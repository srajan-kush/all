#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 2121
#define BUFFER_SIZE 1024

void createDownloadsDirectory() {
    struct stat st = {0};

    if (stat("downloads", &st) == -1) {
        if (mkdir("downloads", 0700) != 0) {
            perror("Error creating downloads directory");
            exit(EXIT_FAILURE);
        }
    }
}

void ftpClient(const std::string& serverIp) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return;
    }

    createDownloadsDirectory();  // Create downloads directory

    while (true) {
        std::string command;
        std::cout << "Enter command (MKDIR, SAVE, RETR, LIST, CD, BACK, DOWNLD or EXIT): ";
        std::getline(std::cin, command);

        if (command == "EXIT") break;

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        if (command.substr(0, 4) == "SAVE") {
            std::string filename = command.substr(5);

            // Check if the file exists on the client
            int fileFd = open(filename.c_str(), O_RDONLY);
            if (fileFd < 0) {
                perror("File not found or inaccessible");
                continue;
            }
            close(fileFd);

            // Send the SAVE command to the server
            send(clientSocket, command.c_str(), command.length(), 0);

            // Reopen the file to send its content
            fileFd = open(filename.c_str(), O_RDONLY);
            if (fileFd < 0) {
                perror("Error reopening the file");
                continue;
            }

            while (true) {
                ssize_t bytesRead = read(fileFd, buffer, BUFFER_SIZE);
                if (bytesRead <= 0) break;
                send(clientSocket, buffer, bytesRead, 0);
            }
            close(fileFd);

            // After sending the file, receive confirmation from the server
            memset(buffer, 0, BUFFER_SIZE);
            recv(clientSocket, buffer, BUFFER_SIZE, 0);
            std::cout << buffer;
        } 
        else if (command.substr(0, 6) == "DOWNLD") {
            std::string filename = command.substr(7);

            // Send the DOWNLD command to the server
            send(clientSocket, command.c_str(), command.length(), 0);

            // Receive server response to start file download
            memset(buffer, 0, BUFFER_SIZE);
            recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (strcmp(buffer, "OK") != 0) {
                std::cout << buffer;
                continue;
            }

            // Save the downloaded file in the downloads directory
            std::string filePath = "downloads/" + filename;
            int fileFd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fileFd < 0) {
                perror("Failed to open file for writing");
                continue;
            }

            ssize_t bytesReceived;
            while (true) {
                bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (bytesReceived <= 0) break;
                if (write(fileFd, buffer, bytesReceived) < 0) {
                    perror("Failed to write to file");
                    close(fileFd);
                    continue;
                }
                // Check if this was the last chunk of data
                if (bytesReceived < BUFFER_SIZE) break;
            }

            // Handle any errors during receiving
            if (bytesReceived < 0) {
                perror("Error receiving file");
                unlink(filePath.c_str()); // Remove partially written file
            }

            close(fileFd);
            std::cout << "File '" << filename << "' downloaded successfully and saved in the 'downloads' directory.\n";
        } else {
            send(clientSocket, command.c_str(), command.length(), 0);

            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            std::cout << buffer;
        }
    }

    close(clientSocket);
}

int main() {
    std::string serverIp;
    std::cout << "Enter server IP address: ";
    std::getline(std::cin, serverIp);

    ftpClient(serverIp);
    return 0;
}

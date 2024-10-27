#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <thread>

#define PORT 2121
#define BUFFER_SIZE (1024)
#define MAX_FILE_SIZE (10 * 1024 * 1024) // 10 MB

class FTPServer {
public:
    FTPServer() {
        // Initialize server socket
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            exit(1);
        }

        // Set up server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        // Bind the socket
        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Binding failed" << std::endl;
            close(server_socket);
            exit(1);
        }

        // Start listening for connections
        listen(server_socket, 5);
        std::cout << "FTP Server is ready to serve client commands..." << std::endl;
    }

    ~FTPServer() {
        close(server_socket);
    }

    void run() {
        while (true) {
            socklen_t addr_len = sizeof(client_addr);
            int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
            if (client_socket < 0) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            // Create a new thread for each client connection
            std::thread client_thread(&FTPServer::handleClient, this, client_socket);
            client_thread.detach(); // Detach the thread so it runs independently
        }
    }

private:
    int server_socket;
    struct sockaddr_in server_addr, client_addr;

    void handleClient(int client_socket) {
        char buffer[BUFFER_SIZE];
        socklen_t addr_len = sizeof(client_addr);
        std::cout << "Client IP address and Port Number: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " connected." << std::endl;

        std::string current_dir = "."; // Start in the current directory

        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) break;

            std::cout << "Client IP address and Port Number: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ": request received " << buffer << std::endl;

            std::string command(buffer);
            if (command.substr(0, 6) == "MKDIR ") {
                std::string dirname = command.substr(6);
                std::string full_path = current_dir + "/" + dirname;
                mkdir(full_path.c_str(), 0755);
                send(client_socket, "Directory created.\n", 20, 0);
            } 
            else if (command.substr(0, 5) == "SAVE ") {
                std::string filename = command.substr(5);
                std::string full_path = current_dir + "/" + filename;

                int fd = open(full_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    send(client_socket, "Failed to save file.\n", 22, 0);
                    continue;
                }

                while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
                    write(fd, buffer, bytes_received);
                    if (bytes_received < BUFFER_SIZE) break; // End of file
                }
                close(fd);
                send(client_socket, "File saved successfully.\n", 25, 0);
            } 
            else if (command.substr(0, 5) == "RETR ") {
                std::string filename = command.substr(4);
                std::string full_path = current_dir + "/" + filename;

                int fd = open(full_path.c_str(), O_RDONLY);
                if (fd < 0) {
                    send(client_socket, "File not found.\n", 17, 0);
                    continue;
                }

                struct stat file_stat;
                if (fstat(fd, &file_stat) < 0) {
                    send(client_socket, "Error getting file size.\n", 26, 0);
                    close(fd);
                    continue;
                }

                if (file_stat.st_size > MAX_FILE_SIZE) {
                    send(client_socket, "File too large to download.\n", 30, 0);
                    close(fd);
                    continue;
                }

                send(client_socket, "OK", 2, 0);
                while ((bytes_received = read(fd, buffer, sizeof(buffer))) > 0) {
                    send(client_socket, buffer, bytes_received, 0);
                }
                close(fd);
            } 
            else if (command == "LIST") {
    DIR *dir = opendir(current_dir.c_str());
    if (!dir) {
        send(client_socket, "Directory not found.\n", 21, 0);
        continue;
    }

    std::string file_list;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') { // Skip hidden files
            file_list += entry->d_name;
            file_list += "\n";
        }
    }
    closedir(dir);

    if (file_list.empty()) { // Check if the list is empty
        send(client_socket, "No files to display.\n", 22, 0);
    } else {
        send(client_socket, file_list.c_str(), file_list.length(), 0);
    }
}

            else if (command.substr(0, 3) == "CD ") {
                std::string dirname = command.substr(3);
                std::string full_path = current_dir + "/" + dirname;

                DIR *dir = opendir(full_path.c_str());
                if (dir) {
                    closedir(dir);
                    current_dir = full_path; // Change to the new directory
                    send(client_socket, "Changed directory.\n", 19, 0);
                } else {
                    send(client_socket, "Directory not found.\n", 21, 0);
                }
            }
            else if (command == "BACK") {
                size_t pos = current_dir.find_last_of('/');
                if (pos != std::string::npos && pos != 0) {
                    current_dir = current_dir.substr(0, pos);
                    if (current_dir.empty()) {
                        current_dir = "."; // Stay in current directory if root
                    }
                    send(client_socket, "Moved back to parent directory.\n", 33, 0);
                } else {
                    send(client_socket, "Already at root directory.\n", 27, 0);
                }
            }
            else if (command.substr(0, 7) == "DOWNLD ") { 
                std::string filename = command.substr(7);
                std::string full_path = current_dir + "/" + filename;

                int fd = open(full_path.c_str(), O_RDONLY);
                if (fd < 0) {
                    send(client_socket, "File not found.\n", 17, 0);
                    continue;
                }

                struct stat file_stat;
                if (fstat(fd, &file_stat) < 0) {
                    send(client_socket, "Error getting file size.\n", 26, 0);
                    close(fd);
                    continue;
                }

                if (file_stat.st_size > MAX_FILE_SIZE) {
                    send(client_socket, "File too large to download.\n", 30, 0);
                    close(fd);
                    continue;
                }

                send(client_socket, "OK", 2, 0);

                ssize_t bytes_read;
                while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
                    if (send(client_socket, buffer, bytes_read, 0) < 0) {
                        perror("Error sending file");
                        break; // Exit the loop on error
                    }
                }

                close(fd);
            }
            else {
                send(client_socket, "Unknown command.\n", 17, 0);
            }
        }
        close(client_socket);
    }
};

int main() {
    FTPServer server;
    server.run();
    return 0;
}
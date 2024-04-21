/*
FTP Server:
    1. Setup Server Socket: Create a TCP server socket and bind it to a port.
    2. Listen for Incoming Connections: Use listen() to start listening for incoming connections.
    3. Accept Connections: Upon receiving a connection request, use accept() to accept the connection and get a new socket descriptor.
    4. Handle Commands: Receive commands from the client on the control channel and execute appropriate actions (get, put, ls, quit).
    5. Data Transfer: Open a new data connection for file transfers as needed.
    6. Implement Protocol: Design and implement the protocol for exchanging messages between the client and server.
    7. Error Handling: Handle errors gracefully and provide appropriate feedback to the client.
    8. Close Connections: Close sockets when the session ends or an error occurs.
*/
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <dirent.h>

const int PORT = 12000;
const int BUFFER_SIZE = 1024;
const char* FILES_FOLDER = "files/";

void sendFile(int clientSocket, const char* fileName) {
    std::ifstream fileStream(fileName, std::ios::binary);
    if (!fileStream.is_open()) {
        const char* errorMessage = "File not found";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
        return;
    }

    char buffer[BUFFER_SIZE];
    while (!fileStream.eof()) {
        fileStream.read(buffer, BUFFER_SIZE);
        send(clientSocket, buffer, fileStream.gcount(), 0);
    }

    fileStream.close();
}

void putFile(int clientSocket, const char* fileName) {
    std::ofstream fileStream(std::string(FILES_FOLDER) + fileName);
    fileStream.close();

    const char* successMessage = "File saved successfully";
    send(clientSocket, successMessage, strlen(successMessage), 0);
}


void listFiles(int clientSocket) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(FILES_FOLDER)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                send(clientSocket, ent->d_name, strlen(ent->d_name), 0);
                send(clientSocket, "\n", 1, 0);
            }
        }
        closedir(dir);
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Bind socket to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    // Listen for connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server running on port " << PORT << std::endl;

    // Accept connections
    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Client connected\n";

        // Handle client commands
        char buffer[BUFFER_SIZE];
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesReceived <= 0) {
                std::cout << "Client disconnected\n";
                break;
            }

            buffer[bytesReceived] = '\0'; // Null-terminate received data

            if (strcmp(buffer, "quit") == 0) {
                std::cout << "Client disconnected\n";
                break;
            } else if (strcmp(buffer, "ls") == 0) {
                listFiles(clientSocket);
            } else if (strncmp(buffer, "get", 3) == 0) {
                const char* fileName = buffer + 4; // Skip "get "
                sendFile(clientSocket, fileName);
            } else if (strncmp(buffer, "put", 3) == 0) {
                const char* fileName = buffer + 4; // Skip "put "
                putFile(clientSocket, fileName);
            }
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

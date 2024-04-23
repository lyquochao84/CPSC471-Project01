/*
FTP Client:
    1. Setup Client Socket: Create a TCP client socket.
    2. Connect to Server: Connect to the server using the server's IP address and port number.
    3. Send Commands: Send commands (get, put, ls, quit) to the server on the control channel.
    4. Handle Responses: Receive responses from the server and display them to the user.
    5. Data Transfer: Open a new data connection for file transfers as needed.
    6. Implement Protocol: Follow the protocol designed for communication with the server.
    7. Error Handling: Handle errors gracefully and provide appropriate feedback to the user.
    8. Close Connections: Close sockets when the session ends or an error occurs.
*/
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>

#define PORT 12000
#define BUFFER_SIZE 1024

using namespace std;

void receiveFile(int clientSocket, const std::string& filename) {
    char buffer[BUFFER_SIZE];

    // Check if the requested file exists
    std::ifstream infile("files/" + filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: File '" << filename << "' not found\n";
        const char* errorMessage = "File not found";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
        return;
    }
    infile.close();

    // Send confirmation to server that file exists
    const char* successMessage = "File found";
    send(clientSocket, successMessage, strlen(successMessage), 0);

    // Get file size
    infile.open("files/" + filename, std::ios::binary | std::ios::ate);
    std::streamsize fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // Send file size
    send(clientSocket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);

    // Receive and save file data
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Could not create file\n";
        return;
    }
    ssize_t bytesRead;
    while (fileSize > 0 && (bytesRead = recv(clientSocket, buffer, std::min(static_cast<std::streamsize>(fileSize), static_cast<std::streamsize>(BUFFER_SIZE)), 0)) > 0) {
      outfile.write(buffer, bytesRead);
      fileSize -= bytesRead;
    }
    outfile.close();

    std::cout << "File " << filename << " saved successfully.\n";
    return;
}


void sendFile(int clientSocket, const string& filename) {
    ifstream infile("files/" + filename, ios::binary);
    if (!infile) {
        cerr << "Error: Could not open file\n";
        return;
    }

    char buffer[BUFFER_SIZE];
    while (infile.good()) {
        infile.read(buffer, BUFFER_SIZE);
        int bytesRead = infile.gcount();
        if (bytesRead > 0) {
            send(clientSocket, buffer, bytesRead, 0);
        }
    }
    infile.close();
}


void listFiles() {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("files")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            cout << ent->d_name << endl;
        }
        closedir(dir);
    } else {
        cerr << "Error: Could not open directory\n";
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error: Could not create socket\n";
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error: Connection failed\n";
        return 1;
    }

    cout << "Connected to server\n";

    // Handle user commands
    string command;
    do {
        cout << "Enter command (get, put, ls, quit): ";
        getline(cin, command);

        // Send command to server
        send(clientSocket, command.c_str(), command.length(), 0);

        // Handle quit command
        if (command == "quit") {
            cout << "quitted" << endl;
            break;
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        string response(buffer);

        // Display response from server
        cout << "Server response: \n" << response << endl;

        // Handle get command
        if (command.substr(0, 3) == "get") {
            receiveFile(clientSocket, command.substr(4));
        }
        // Handle put command
        else if (command.substr(0, 3) == "put") {
            sendFile(clientSocket, command.substr(4));
        }
        // Handle ls command
        else if (command.substr(0, 2) == "ls") {
            listFiles();
        }
    } while (true);

    close(clientSocket);
    return 0;
}


# CPSC 471 Programming Assignment 01


## Group Members
* Hao Ly (hly66@csu.fullerton.edu)
* Chingsan Ly (cly53@csu.fullerton.edu)
* Tristan Do  (tdo297@csu.fullerton.edu)
## Description
- Implement FTP Server and FTP Client
- The client shall connect to the server and support uploading and downloading of files to/from server



## Programming Language
* C++
## Operating System
- Linux
## Protocol Desgin

+ Understanding FTP Basics:
* FTP is a standard network protocol used for the transfer of computer files between a client and server on a computer network.
* It operates on the client-server model, where the client initiates a connection to the server to perform file transfers.
* FTP typically runs on port 21 for control connections and on port 20 for data connections, although these ports can be configured.

+ Components of FTP:
* Client: The user interface that allows users to interact with the FTP server. Clients can be command-line based, like ftp command in Unix/Linux systems, or graphical, such as FileZilla and WinSCP.
* Server: A computer system running FTP server software, which provides access to files and directories stored on it to clients.
* Control Connection: This is the initial connection established between the client and server. It is used for sending commands, such as login credentials and requests for file transfers.
* Data Connection: Separate connections are established for data transfer. These connections are used to transfer the actual files between the client and server.

+ FTP Commands:
* FTP operates using a set of commands that clients can send to servers to perform various operations. Some common commands include:
- USER: Specifies the username for authentication.
- PASS: Specifies the password for authentication.
- LIST: Retrieves a list of files in the current directory.
- GET: Downloads a file from the server to the client.
- PUT: Uploads a file from the client to the server.
- PWD: Prints the current working directory.
- QUIT: Terminates the FTP session.

+ Security Considerations:
* FTP originally lacked encryption, transmitting data, including usernames and passwords, in plain text. This posed significant security risks.
* Secure FTP (SFTP) and FTPS were introduced to address these security concerns. SFTP runs over SSH and provides encryption for both control and data connections, while FTPS is FTP with SSL/TLS encryption.

+ Implementations:
* There are various implementations of FTP servers and clients available, both open-source and commercial. Some popular ones include:
- ProFTPD: An open-source FTP server for Unix/Linux systems.
- vsftpd: Another lightweight, open-source FTP server for Unix/Linux systems.
- FileZilla: A popular, open-source FTP client available for multiple platforms.
- WinSCP: A free, open-source SFTP, SCP, and FTP client for Windows

+ Further Resources:
* Reading RFC 959, which defines the FTP protocol, can provide a comprehensive understanding of its workings.
* Online tutorials, books, and documentation are also valuable resources for learning about FTP.


## Execute Program
**1) Compile and Start the Server**
```
g++ ftp-server.cpp -o server 
./server
```
**Note**: The server will start on port 12000

**2) Compile and Start the Client**
```
g++ ftp-client.cpp -o client 
./client
```
**Note**: The client will connect to the server on port 12000.

**3) Client-Side Command**
- **put**: `put <filename>`
    - Saves the file created by the user through the terminal with the put command to the **files** folder on the user's computer.
    - Notifies the user with `File saved successfully` in the terminal.

- **get**: `get <filename>`
    - Downloads the specified file from the server to the user's computer.

- **ls**: `ls`
    - Displays all the files in the **files** folder on the user's computer.

- **quit**: `quit`
    - Exits the program on the client-side.
    - Notifies the server with `Client disconnected`



## NOTE
- Ensure that both the server and client are executed in separate terminal.
    - Linux terminal
    - If you're using VSCode, make sure using **Ubuntu(WSL)** terminal to execute the program
- The server must be started before running the client.
- All file operations are performed in the **files** folder.

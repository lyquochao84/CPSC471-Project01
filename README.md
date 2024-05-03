
# CPSC 471 Programming Assignment 01


## Group Members
* Hao Ly (hly66@csu.fullerton.edu)
* Chingsan Ly (cly53@csu.fullerton.edu)
* Tristan Do (tdo297@csu.fullerton.edu)
## Description
- Implement FTP Server and FTP Client
- The client shall connect to the server and support uploading and downloading of files to/from server



## Programming Language
* C++
## Operating System
- Linux
## Protocol Desgin
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

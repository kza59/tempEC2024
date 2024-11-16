#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;

string name;
int PORT;

void sending() {
    char buffer[10000] = {0};

    int PORT_server;
    cout << "Enter the port to send a message: ";
    cin >> PORT_server;

    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_server);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return;
    }

    cin.ignore();
    cout << "Enter your message: ";
    cin.getline(message, 1024);

    snprintf(buffer, sizeof(buffer), "%s [PORT:%d] says: %s", name.c_str(), PORT, message);
    send(sock, buffer, strlen(buffer), 0);
    cout << "Message sent!" << endl;

    close(sock);
}

void receiving(int SFD) {
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    char buffer[10000] = {0}; // Buffer for incoming messages
    fd_set currentSockets, readySockets;

    FD_ZERO(&currentSockets);
    FD_SET(SFD, &currentSockets);

    for (;;) {
        readySockets = currentSockets;

        if (select(FD_SETSIZE, &readySockets, NULL, NULL, NULL) < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &readySockets)) {
                if (i == SFD) {
    
                    int clientSocket = accept(SFD, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
                    if (clientSocket < 0) {
                        perror("Accept error");
                        continue;
                    }
                    FD_SET(clientSocket, &currentSockets);
                } else {

                    int value = recv(i, buffer, sizeof(buffer), 0);
                    if (value <= 0) {
                        close(i);
                        FD_CLR(i, &currentSockets);
                    } else {
                        cout << "Received: " << buffer << endl;
                        memset(buffer, 0, sizeof(buffer)); // Clear buffer
                    }
                }
            }
        }
    }
}

void *receiveThread(void *serverFileDes) {
    int SFD = *((int *)serverFileDes);
    receiving(SFD);
    return nullptr;
}

int main() {
    int serverFileDes;
    struct sockaddr_in addr;

    cout << "Enter your name: ";
    cin >> name;

    cout << "Enter your port number: ";
    cin >> PORT;

    if ((serverFileDes = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(serverFileDes, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(serverFileDes);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFileDes, 5) < 0) {
        perror("Listen failed");
        close(serverFileDes);
        exit(EXIT_FAILURE);
    }

    pthread_t threadID;
    pthread_create(&threadID, NULL, receiveThread, &serverFileDes);


    while (true) {
        string input;
        cout << "Enter a command (send/s, quit/q): ";
        cin >> input;

        for (auto &ch : input) {
            ch = tolower(ch);
        }

        if (input == "quit" || input == "q") {
            cout << "Quitting..." << endl;
            close(serverFileDes);
            break;
        } else if (input == "send" || input == "s") {
            sending();
        } else {
            cout << "Invalid command!" << endl;
        }
    }

    pthread_join(threadID, NULL); 
    cout << "Peer-to-peer communication ended." << endl;
    return 0;
}

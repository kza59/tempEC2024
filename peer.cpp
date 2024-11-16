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

void sending() {



}
void receiving() {


    
}
int main() {
    string name = "";
    int portNum = 0;
    int serverFileDes, newSocket, valueRead;
    struct sockaddr_in addr;


    //getting name
    cout << "enter name" << endl;
    cin >> name;
    cout << "name instantiated as: " << name << endl;

    //getting port number
    cout << "enter port number" << endl;
    cin >> portNum;
    cout << "port number instantiated as: " << portNum << endl;

    //instantiating the server file descriptor
    serverFileDes = socket(AF_INET, SOCK_STREAM, 0);
    cout << "server file descriptor instantiated as: " << serverFileDes << endl;
    // if(serverFileDes == -1) {
    //     errno = EBADF;
    //     exit(EXIT_FAILURE);
    // }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    //converts to network byte order
    addr.sin_port = htons(portNum);

    if(bind(serverFileDes, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("SERVER FILE DESCRIPTOR BINDING FAILED");
        exit(EXIT_FAILURE);
    }
    if(listen(serverFileDes, 5) < 0) {
        perror("LISTEN");
        exit(EXIT_FAILURE);
    }
    cout << "fin" << endl;


    while(true) {
    string input = "";
    cout << "enter an input. This input can either be 0(quit/q) or 1(send/s)" << endl;
    cin >> input;
    // tolower(input);
    for(auto& item : input) {
        tolower(item);
    }
    if(input == "quit" || input == "q" || input == "0") {
        cout << "quitting" << endl;
        close(serverFileDes);
        break;
    }
    else if(input == "send" || input == "s" || input == "1") {
        cout << "sending" << endl;
        sending();
    }

    }
    cout << "ended the p2p communication" << endl;





    // if((serverFileDes = so))




}
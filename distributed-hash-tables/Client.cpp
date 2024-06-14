#include "Client.h"


void showMenu() {
    cout << "************************MENU*************************" << endl;
    cout << "PRESS ***********************************************" << endl;
    cout << "1. TO ENTER *****************************************" << endl;
    cout << "2. TO SHOW ******************************************" << endl;
    cout << "3. TO DELETE ****************************************" << endl;
    cout << "4. TO EXIT ******************************************" << endl;
    cout << "*****************************************************" << endl;
}

void handleInsert(int sock) {
    string key, val;
    cout << "ENTER THE KEY: ";
    cin >> key;
    cout << "ENTER THE VALUE: ";
    cin >> val;
    string message = "insert|" + key + ":" + val;
    send(sock, message.c_str(), message.size(), 0);
    char buffer[1024] = {0};
    recv(sock, buffer, 1024, 0);
    cout << buffer << endl;
}

void handleSearch(int sock) {
    string key;
    cout << "ENTER THE KEY: ";
    cin >> key;
    string message = "search|" + key;
    send(sock, message.c_str(), message.size(), 0);
    char buffer[1024] = {0};
    recv(sock, buffer, 1024, 0);
    cout << "The value corresponding to the key is: " << buffer << endl;
}

void handleDelete(int sock) {
    string key;
    cout << "ENTER THE KEY: ";
    cin >> key;
    string message = "delete|" + key;
    send(sock, message.c_str(), message.size(), 0);
    char buffer[1024] = {0};
    recv(sock, buffer, 1024, 0);
    cout << buffer << endl;
}

int main() {
    string ip = "127.0.0.1";
    int port;

    cout << "Give the port number of a node: ";
    cin >> port;

    while (true) {
        showMenu();
        string choice;
        cin >> choice;

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            cerr << "Socket creation failed" << endl;
            return 1;
        }

        sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);

        if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            cerr << "Connection failed" << endl;
            close(sock);
            return 1;
        }

        if (choice == "1") {
            handleInsert(sock);
        } else if (choice == "2") {
            handleSearch(sock);
        } else if (choice == "3") {
            handleDelete(sock);
        } else if (choice == "4") {
            cout << "Closing the socket" << endl;
            close(sock);
            cout << "Exiting Client" << endl;
            exit(0);
        } else {
            cout << "INCORRECT CHOICE" << endl;
        }

        close(sock);
    }

    return 0;
}

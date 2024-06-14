
#include "RequestHandler.h"

string RequestHandler::send_message(const string& ip, int port, const string& message) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Socket creation failed\n";
        return "";
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cerr << "Connection failed\n";
        return "";
    }

    send(sock, message.c_str(), message.size(), 0);
    char buffer[1024] = {0};
    recv(sock, buffer, 1024, 0);

    close(sock);
    return string(buffer);
}

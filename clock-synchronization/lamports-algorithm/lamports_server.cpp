#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

void handle_client(tcp::socket& socket) {
    try {
        int lamport_clock = 0;
        boost::asio::read(socket, boost::asio::buffer(&lamport_clock, sizeof(lamport_clock)));
        lamport_clock++;
        boost::asio::write(socket, boost::asio::buffer(&lamport_clock, sizeof(lamport_clock)));
    } catch (exception& e) {
        cerr << "Error handling client: " << e.what() << endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12349));

        cout << "Lamport's Algorithm Server is running..." << endl;

        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            thread(handle_client, std::ref(socket)).detach();
        }
    } catch (exception& e) {
        cerr << "Server error: " << e.what() << endl;
    }
    return 0;
}

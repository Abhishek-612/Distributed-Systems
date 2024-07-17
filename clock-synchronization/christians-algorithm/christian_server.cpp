#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

void handle_client(tcp::socket& socket) {
    try {
        auto server_time = system_clock::now();
        auto duration = duration_cast<milliseconds>(server_time.time_since_epoch()).count();
        boost::asio::write(socket, boost::asio::buffer(&duration, sizeof(duration)));
    } catch (exception& e) {
        cerr << "Error handling client: " << e.what() << endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));

        cout << "Christian's Algorithm Server is running..." << endl;

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

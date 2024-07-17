#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

void handle_client(tcp::socket& socket) {
    try {
        int64_t t1, t2, t3, t4;
        boost::asio::read(socket, boost::asio::buffer(&t1, sizeof(t1)));

        auto server_receive_time = system_clock::now();
        t2 = duration_cast<milliseconds>(server_receive_time.time_since_epoch()).count();

        auto server_send_time = system_clock::now();
        t3 = duration_cast<milliseconds>(server_send_time.time_since_epoch()).count();

        boost::asio::write(socket, boost::asio::buffer(&t2, sizeof(t2)));
        boost::asio::write(socket, boost::asio::buffer(&t3, sizeof(t3)));
    } catch (exception& e) {
        cerr << "Error handling client: " << e.what() << endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12347));

        cout << "NTP Server is running..." << endl;

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

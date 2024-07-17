#include <iostream>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

const int NUM_PROCESSES = 3;

void handle_client(tcp::socket& socket, int process_id) {
    try {
        vector<int> vector_clock(NUM_PROCESSES, 0);
        boost::asio::read(socket, boost::asio::buffer(vector_clock.data(), vector_clock.size() * sizeof(int)));
        vector_clock[process_id]++;
        boost::asio::write(socket, boost::asio::buffer(vector_clock.data(), vector_clock.size() * sizeof(int)));
    } catch (exception& e) {
        cerr << "Error handling client: " << e.what() << endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12348));

        cout << "Vector Clocks Server is running..." << endl;

        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            thread(handle_client, std::ref(socket), 0).detach(); // Using process_id 0 for simplicity
        }
    } catch (exception& e) {
        cerr << "Server error: " << e.what() << endl;
    }
    return 0;
}

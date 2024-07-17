#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

const int NUM_PROCESSES = 3;

void vector_clocks_client(const string& host) {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "12348");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        vector<int> vector_clock(NUM_PROCESSES, 0);
        vector_clock[1]++; // Increment client's own clock

        boost::asio::write(socket, boost::asio::buffer(vector_clock.data(), vector_clock.size() * sizeof(int)));

        boost::system::error_code error;
        boost::asio::read(socket, boost::asio::buffer(vector_clock.data(), vector_clock.size() * sizeof(int)), error);
        if (error == boost::asio::error::eof){}


        cout << "Updated Vector Clock: ";
        for (const auto& time : vector_clock) {
            cout << time << " ";
        }
        cout << endl;
    } catch (exception& e) {
        cerr << "Client error: " << e.what() << endl;
    }
}

int main() {
    vector_clocks_client("127.0.0.1");
    return 0;
}

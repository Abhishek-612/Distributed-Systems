#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

void lamports_client(const string& host) {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "12349");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        int lamport_clock = 1; // Initialize client's Lamport clock
        boost::asio::write(socket, boost::asio::buffer(&lamport_clock, sizeof(lamport_clock)));
        
        boost::system::error_code error;
        boost::asio::read(socket, boost::asio::buffer(&lamport_clock, sizeof(lamport_clock)), error);
        if (error == boost::asio::error::eof){}


        cout << "Updated Lamport Clock: " << lamport_clock << endl;
    } catch (exception& e) {
        cerr << "Client error: " << e.what() << endl;
    }
}

int main() {
    lamports_client("127.0.0.1");
    return 0;
}

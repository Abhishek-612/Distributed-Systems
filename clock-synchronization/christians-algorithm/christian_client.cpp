#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

void christian_algorithm(const string& host) {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "12345");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        auto client_send_time = system_clock::now();
        int64_t client_send_time_ms = duration_cast<milliseconds>(client_send_time.time_since_epoch()).count();

        boost::system::error_code error;
        int64_t server_time;
        boost::asio::read(socket, boost::asio::buffer(&server_time, sizeof(server_time)), error);
        
        if (error == boost::asio::error::eof){}

        auto client_receive_time = system_clock::now();
        int64_t client_receive_time_ms = duration_cast<milliseconds>(client_receive_time.time_since_epoch()).count();

        auto round_trip_delay = client_receive_time_ms - client_send_time_ms;
        auto offset = ((server_time - client_send_time_ms) - round_trip_delay / 2);

        cout << "Client send time: " << client_send_time_ms << " ms" << endl;
        cout << "Server time: " << server_time << " ms" << endl;
        cout << "Client receive time: " << client_receive_time_ms << " ms" << endl;
        cout << "Round trip delay: " << round_trip_delay << " ms" << endl;
        cout << "Clock offset: " << offset << " ms" << endl;

        // Adjust client clock (for simulation purposes, just display the adjusted time)
        auto adjusted_time = client_send_time + milliseconds(offset);
        cout << "Adjusted client time: " << duration_cast<milliseconds>(adjusted_time.time_since_epoch()).count() << " ms" << endl;
    } catch (exception& e) {
        cerr << "Client error: " << e.what() << endl;
    }
}

int main() {
    christian_algorithm("127.0.0.1");
    return 0;
}

#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

void berkeley_client(const string& host) {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "12346");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        boost::system::error_code error;

        auto node_time = system_clock::now();
        int64_t node_time_ms = duration_cast<milliseconds>(node_time.time_since_epoch()).count();
        boost::asio::write(socket, boost::asio::buffer(&node_time_ms, sizeof(node_time_ms)), error);

        if (error == boost::asio::error::eof){}

        int average_offset;
        boost::asio::read(socket, boost::asio::buffer(&average_offset, sizeof(average_offset)), error);

        if (error == boost::asio::error::eof){}

        cout << "Node time: " << node_time_ms << " ms" << endl;
        cout << "Average offset: " << average_offset << " ms" << endl;

        // Adjust node clock (for simulation purposes, just display the adjusted time)
        auto adjusted_node_time = node_time + milliseconds(average_offset);
        cout << "Adjusted node time: " << duration_cast<milliseconds>(adjusted_node_time.time_since_epoch()).count() << " ms" << endl;
    } catch (exception& e) {
        cerr << "Client error: " << e.what() << endl;
    }
}

int main() {
    berkeley_client("127.0.0.1");
    return 0;
}

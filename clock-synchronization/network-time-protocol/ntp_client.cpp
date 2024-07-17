#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

void ntp_client(const string& host) {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, "12347");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        auto t1 = system_clock::now();
        int64_t t1_ms = duration_cast<milliseconds>(t1.time_since_epoch()).count();
        boost::asio::write(socket, boost::asio::buffer(&t1_ms, sizeof(t1_ms)));

        boost::system::error_code error;

        int64_t t2, t3;
        boost::asio::read(socket, boost::asio::buffer(&t2, sizeof(t2)), error);
        if (error == boost::asio::error::eof){}

        boost::asio::read(socket, boost::asio::buffer(&t3, sizeof(t3)), error);
        if (error == boost::asio::error::eof){}

        auto t4 = system_clock::now();
        int64_t t4_ms = duration_cast<milliseconds>(t4.time_since_epoch()).count();

        auto offset = ((t2 - t1_ms) + (t3 - t4_ms)) / 2;
        auto delay = (t4_ms - t1_ms) - (t3 - t2);
        cout << "Client send time (T1): " << t1_ms << " ms" << endl;
        cout << "Server receive time (T2): " << t2 << " ms" << endl;
        cout << "Server send time (T3): " << t3 << " ms" << endl;
        cout << "Client receive time (T4): " << t4_ms << " ms" << endl;

        cout << "Offset: " << offset << " ms" << endl;
        cout << "Round trip delay: " << delay << " ms" << endl;

        // Adjust client clock (for simulation purposes, just display the adjusted time)
        auto adjusted_time = t1 + milliseconds(offset);
        cout << "Adjusted client time: " << duration_cast<milliseconds>(adjusted_time.time_since_epoch()).count() << " ms" << endl;
    } catch (exception& e) {
        cerr << "Client error: " << e.what() << endl;
    }
}

int main() {
    ntp_client("127.0.0.1");
    return 0;
}

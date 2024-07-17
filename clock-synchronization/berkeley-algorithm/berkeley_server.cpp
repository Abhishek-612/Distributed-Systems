#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;
using namespace chrono;

vector<time_point<system_clock>> get_node_times(tcp::acceptor& acceptor, int num_nodes) {
    vector<time_point<system_clock>> node_times;
    for (int i = 0; i < num_nodes; ++i) {
        tcp::socket socket(acceptor.get_executor());
        acceptor.accept(socket);
        int64_t node_time;
        boost::asio::read(socket, boost::asio::buffer(&node_time, sizeof(node_time)));
        node_times.push_back(system_clock::time_point(milliseconds(node_time)));
    }
    return node_times;
}

void send_adjustments(tcp::acceptor& acceptor, int num_nodes, int average_offset) {
    for (int i = 0; i < num_nodes; ++i) {
        tcp::socket socket(acceptor.get_executor());
        acceptor.accept(socket);
        boost::asio::write(socket, boost::asio::buffer(&average_offset, sizeof(average_offset)));
    }
}

void berkeley_server(int num_nodes) {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12346));

        cout << "Berkeley Algorithm Server is running..." << endl;

        auto master_time = system_clock::now();
        auto node_times = get_node_times(acceptor, num_nodes);

        vector<int> offsets;
        for (const auto& node_time : node_times) {
            offsets.push_back(duration_cast<milliseconds>(node_time - master_time).count());
        }

        int sum_offsets = 0;
        for (const auto& offset : offsets) {
            sum_offsets += offset;
        }

        int average_offset = sum_offsets / (num_nodes + 1); // Include master in the average

        cout << "Master time: " << duration_cast<milliseconds>(master_time.time_since_epoch()).count() << " ms" << endl;
        for (int i = 0; i < num_nodes; ++i) {
            cout << "Node " << i + 1 << " time: " << duration_cast<milliseconds>(node_times[i].time_since_epoch()).count() << " ms" << endl;
        }

        cout << "Average offset: " << average_offset << " ms" << endl;

        send_adjustments(acceptor, num_nodes, average_offset);

        // Adjust master clock (for simulation purposes, just display the adjusted time)
        auto adjusted_master_time = master_time + milliseconds(average_offset);
        cout << "Adjusted master time: " << duration_cast<milliseconds>(adjusted_master_time.time_since_epoch()).count() << " ms" << endl;
    } catch (exception& e) {
        cerr << "Server error: " << e.what() << endl;
    }
}

int main() {
    int num_nodes = 5;
    berkeley_server(num_nodes);
    return 0;
}

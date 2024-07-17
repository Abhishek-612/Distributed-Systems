#!/bin/bash

# Function to start server in the background
start_server() {
    local server_program=$1
    nohup ./$server_program > /dev/null 2>&1 &
    local server_pid=$!
    echo $server_pid
}

# Function to run client and capture output
run_client() {
    local client_program=$1
    local output_file=$2
    local retries=5
    local wait_time=2

    for i in $(seq 1 $retries); do
        if ./$client_program > $output_file; then
            return 0
        else
            echo "Client failed to connect, retrying in $wait_time seconds..."
            sleep $wait_time
        fi
    done

    echo "Client failed to connect after $retries attempts."
    return 1
}

# Function to kill server
kill_server() {
    local server_pid=$1
    kill $server_pid
}

make clean
make

CHRISTIAN_SERVER="christians-algorithm/christian_server"
CHRISTIAN_CLIENT="christians-algorithm/christian_client"
BERKELEY_SERVER="berkeley-algorithm/berkeley_server"
BERKELEY_CLIENT="berkeley-algorithm/berkeley_client"
NTP_SERVER="network-time-protocol/ntp_server"
NTP_CLIENT="network-time-protocol/ntp_client"
VECTOR_CLOCKS_SERVER="vector-clocks/vector_clocks_server"
VECTOR_CLOCKS_CLIENT="vector-clocks/vector_clocks_client"
LAMPORTS_SERVER="lamports-algorithm/lamports_server"
LAMPORTS_CLIENT="lamports-algorithm/lamports_client"

# Output log file
LOG_FILE="performance_logs.txt"

rm -f $LOG_FILE

test_performance() {
    local server_program=$1
    local client_program=$2
    local num_iterations=$3

    echo "Testing $server_program and $client_program" | tee -a $LOG_FILE

    for i in $(seq 1 $num_iterations); do
        echo "Iteration $i" | tee -a $LOG_FILE

        server_pid=$(start_server $server_program)
        sleep 1 # Wait for the server to start

        output_file="output_$i.txt"
        if run_client $client_program $output_file; then
            cat $output_file | tee -a $LOG_FILE
        else
            echo "Client failed to connect after retries. Skipping iteration." | tee -a $LOG_FILE
        fi

        kill_server $server_pid

        rm $output_file

        echo "" | tee -a $LOG_FILE
    done
}

NUM_ITERATIONS=10

test_performance $CHRISTIAN_SERVER $CHRISTIAN_CLIENT $NUM_ITERATIONS
test_performance $BERKELEY_SERVER $BERKELEY_CLIENT $NUM_ITERATIONS
test_performance $NTP_SERVER $NTP_CLIENT $NUM_ITERATIONS
test_performance $VECTOR_CLOCKS_SERVER $VECTOR_CLOCKS_CLIENT $NUM_ITERATIONS
test_performance $LAMPORTS_SERVER $LAMPORTS_CLIENT $NUM_ITERATIONS

echo "Performance testing completed. Results saved in $LOG_FILE"

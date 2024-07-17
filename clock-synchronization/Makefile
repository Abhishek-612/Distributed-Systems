
BOOST_INCLUDE_PATH=/opt/homebrew/include
BOOST_LIB_PATH=/opt/homebrew/lib/

CXX = g++
CXXFLAGS = -std=c++11 -O2 -I$(BOOST_INCLUDE_PATH)
LDFLAGS = -L$(BOOST_LIB_PATH) -lboost_system -pthread

CHRISTIAN_SERVER = christians-algorithm/christian_server
CHRISTIAN_CLIENT = christians-algorithm/christian_client

BERKELEY_SERVER = berkeley-algorithm/berkeley_server
BERKELEY_CLIENT = berkeley-algorithm/berkeley_client

NTP_SERVER = network-time-protocol/ntp_server
NTP_CLIENT = network-time-protocol/ntp_client

VECTOR_CLOCKS_SERVER = vector-clocks/vector_clocks_server
VECTOR_CLOCKS_CLIENT = vector-clocks/vector_clocks_client

LAMPORTS_SERVER = lamports-algorithm/lamports_server
LAMPORTS_CLIENT = lamports-algorithm/lamports_client

# Source files
CHRISTIAN_SERVER_SRC = christians-algorithm/christian_server.cpp
CHRISTIAN_CLIENT_SRC = christians-algorithm/christian_client.cpp

BERKELEY_SERVER_SRC = berkeley-algorithm/berkeley_server.cpp
BERKELEY_CLIENT_SRC = berkeley-algorithm/berkeley_client.cpp

NTP_SERVER_SRC = network-time-protocol/ntp_server.cpp
NTP_CLIENT_SRC = network-time-protocol/ntp_client.cpp

VECTOR_CLOCKS_SERVER_SRC = vector-clocks/vector_clocks_server.cpp
VECTOR_CLOCKS_CLIENT_SRC = vector-clocks/vector_clocks_client.cpp

LAMPORTS_SERVER_SRC = lamports-algorithm/lamports_server.cpp
LAMPORTS_CLIENT_SRC = lamports-algorithm/lamports_client.cpp


TARGETS = $(CHRISTIAN_SERVER) $(CHRISTIAN_CLIENT) \
          $(BERKELEY_SERVER) $(BERKELEY_CLIENT) \
          $(NTP_SERVER) $(NTP_CLIENT) \
          $(VECTOR_CLOCKS_SERVER) $(VECTOR_CLOCKS_CLIENT) \
          $(LAMPORTS_SERVER) $(LAMPORTS_CLIENT)


all: $(TARGETS)

$(CHRISTIAN_SERVER): $(CHRISTIAN_SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(CHRISTIAN_CLIENT): $(CHRISTIAN_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BERKELEY_SERVER): $(BERKELEY_SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BERKELEY_CLIENT): $(BERKELEY_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(NTP_SERVER): $(NTP_SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(NTP_CLIENT): $(NTP_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(VECTOR_CLOCKS_SERVER): $(VECTOR_CLOCKS_SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(VECTOR_CLOCKS_CLIENT): $(VECTOR_CLOCKS_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(LAMPORTS_SERVER): $(LAMPORTS_SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(LAMPORTS_CLIENT): $(LAMPORTS_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	rm -f $(TARGETS)


.PHONY: all clean

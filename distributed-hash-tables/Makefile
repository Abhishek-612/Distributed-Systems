CFLAGS = -Wall -g -std=c++11
LDFLAGS = -L/opt/homebrew/opt/openssl@1.1/lib
CPPFLAGS = -I/opt/homebrew/opt/openssl@1.1/include

all: Application Client

Application: DataStore.o Node.o Application.o ChordProtocol.o RequestHandler.o
	g++ -o Application DataStore.o Node.o Application.o ChordProtocol.o RequestHandler.o ${CFLAGS} ${LDFLAGS} -lssl -lcrypto

Client: Client.o
	g++ -o Client Client.o ${CFLAGS}

DataStore.o: DataStore.cpp DataStore.h
	g++ -c DataStore.cpp ${CFLAGS} ${CPPFLAGS}

Node.o: Node.cpp Node.h DataStore.h ChordProtocol.h RequestHandler.h
	g++ -c Node.cpp ${CFLAGS} ${CPPFLAGS}

Application.o: Application.cpp Application.h
	g++ -c Application.cpp ${CFLAGS} ${CPPFLAGS}

ChordProtocol.o: ChordProtocol.cpp ChordProtocol.h Node.h
	g++ -c ChordProtocol.cpp ${CFLAGS} ${CPPFLAGS}

RequestHandler.o: RequestHandler.cpp RequestHandler.h
	g++ -c RequestHandler.cpp ${CFLAGS} ${CPPFLAGS}

Client.o: Client.cpp
	g++ -c Client.cpp ${CFLAGS} ${CPPFLAGS}

clean:
	rm -rf *.o Application Client

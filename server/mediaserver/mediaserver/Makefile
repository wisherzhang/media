CC=g++ -fpic

mediaserver: peer_channel.o utils.o config.o data_socket.o main.o
	${CC} peer_channel.cc -o peer_channel.o
	${CC} utils.cc -o utils.o
	${CC} config.cpp -o config.o
	${CC} data_socket.cc -o data_socket.o
	${CC} main.cpp -o main.o
clean:
	rm *.o

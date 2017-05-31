CC=g++
CCFLAG=-pie
TARGET=mediaserver
objects=main.cc peer_channel.cc data_socket.cc utils.cc config.cc death_handler.cc
mediaserver: ${objects}
	${CC} -o  ${TARGET} -std=c++11 ${objects} -ldl
 
clean:
	rm *.o ${TARGET} 

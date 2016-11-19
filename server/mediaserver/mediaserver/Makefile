CC=g++
CCFLAG=-c -pie
objects=main.o peer_channel.o data_socket.o utils.o config.o
mediaserver: ${objects}
	${CC} -o  mediaserver  ${objects}
	${CC} ${CCFLAG} main.cpp
	${CC} ${CCFLAG} peer_channel.cc
	${CC} ${CCFLAG} data_socket.cc
	${CC} ${CCFLAG} utils.cc 
	${CC} ${CCFLAG} config.cpp

 
clean:
	rm *.o

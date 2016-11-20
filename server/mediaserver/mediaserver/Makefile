CC=g++
CCFLAG=-c -pie
objects=main.o peer_channel.o data_socket.o utils.o config.o
mediaserver: ${objects}
	${CC} -o  mediaserver  ${objects}
 
clean:
	rm *.o

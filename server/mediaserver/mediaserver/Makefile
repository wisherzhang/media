CC=g++
CCFLAG=-c -pie
objects=main.cc peer_channel.cc data_socket.cc utils.cc config.cc death_handler.cc
mediaserver: ${objects}
	${CC} -o  mediaserver  ${objects} -ldl
 
clean:
	rm *.o

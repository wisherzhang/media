CC=g++
CCFLAG=-c -pie
objects=tcp_client.o udp_client.o
client: ${objects}
	${CC} -o  client  ${objects}
	${CC} ${CCFLAG} tcp_client.cpp
	${CC} ${CCFLAG} udp_client.cpp
 
clean:
	rm *.o

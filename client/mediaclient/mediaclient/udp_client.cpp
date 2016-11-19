#include "common.h"

int main_udp(int argc, char* argv[])
{
#ifdef WIN32
	WORD socketVersion = MAKEWORD(2,2);
	WSADATA wsaData; 
	if(WSAStartup(socketVersion, &wsaData) != 0)
	{
		return 0;
	}
#endif

	NativeSocket sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1"); 
	socklen_t len = sizeof(sin);

	const char * sendData = "来自客户端的数据包.\n";
	sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

	char recvData[255]; 	
	int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
	if(ret > 0)
	{
		recvData[ret] = 0x00;
		printf("%s",recvData);
	}

	closesocket(sclient);
#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}

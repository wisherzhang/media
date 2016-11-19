#include "common.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
	// 加载socket动态链接库(dll)  
	WORD sockVersion = MAKEWORD(2,2); // 请求2.2版本的WinSock库 
	WSADATA data; // 这结构是用于接收Wjndows Socket的结构信息的
	if(WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
#endif
	
	NativeSocket sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	const char * sendData = "你好，TCP服务端，我是客户端!\n";
	send(sclient, sendData, strlen(sendData), 0);

	char recData[255];
	int ret = recv(sclient, recData, 255, 0);
	if(ret > 0)
	{
		recData[ret] = 0x00;
		printf("%s",recData);
	}
	closesocket(sclient);
#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}

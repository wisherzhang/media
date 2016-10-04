/******************************
created in 2016.09.29
******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include "data_socket.h"
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 1024
//udp
int udp_server(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, 0); 
	if(serSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("bind error !");
		closesocket(serSocket);
		return 0;
	}

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr); 
	while (true)
	{
		char recvData[255];  
		int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
		if (ret > 0)
		{
			recvData[ret] = 0x00;
			printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			printf(recvData);			
		}

		char * sendData = "一个来自服务端的UDP数据包\n";
		sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);	

	}
	closesocket(serSocket); 
	WSACleanup();
	return 0;
}
//udp
int main(int argc, char* argv[])
{
	////初始化WSA
	//WORD sockVersion = MAKEWORD(2,2);
	//WSADATA wsaData;
	//if(WSAStartup(sockVersion, &wsaData)!=0)
	//{
	//	return 0;
	//}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, 0);
	if(slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; 
	if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听
	if(listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255]; 
	while (true)
	{
		//printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		//printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据
		int ret = recv(sClient, revData, 255, 0);		
		if(ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//发送数据
		//char * sendData = "你好，TCP客户端！\n";
		send(sClient, revData, strlen(revData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	//WSACleanup();
	return 0;
}

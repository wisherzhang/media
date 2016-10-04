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
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			printf(recvData);			
		}

		char * sendData = "һ�����Է���˵�UDP���ݰ�\n";
		sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);	

	}
	closesocket(serSocket); 
	WSACleanup();
	return 0;
}
//udp
int main(int argc, char* argv[])
{
	////��ʼ��WSA
	//WORD sockVersion = MAKEWORD(2,2);
	//WSADATA wsaData;
	//if(WSAStartup(sockVersion, &wsaData)!=0)
	//{
	//	return 0;
	//}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, 0);
	if(slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; 
	if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����
	if(listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255]; 
	while (true)
	{
		//printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		//printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������
		int ret = recv(sClient, revData, 255, 0);		
		if(ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//��������
		//char * sendData = "��ã�TCP�ͻ��ˣ�\n";
		send(sClient, revData, strlen(revData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	//WSACleanup();
	return 0;
}

/******************************
created in 2016.09.29
******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <vector>
#include "data_socket.h"
#include "peer_channel.h"
#include "config.h"
using namespace std;
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
#define BUF_SIZE 1024
static const size_t kMaxConnections = (FD_SETSIZE - 2);
#include <assert.h>
void HandleBrowserRequest(DataSocket* ds, bool* quit) {
	assert(ds && ds->valid());
	assert(quit);

	const std::string& path = ds->request_path();

	*quit = (path.compare("/quit") == 0);

	if (*quit) {
		ds->Send("200 OK", true, "text/html", "",
			"<html><body>Quitting...</body></html>");
	} else if (ds->method() == DataSocket::OPTIONS) {
		// We'll get this when a browsers do cross-resource-sharing requests.
		// The headers to allow cross-origin script support will be set inside
		// Send.
		ds->Send("200 OK", true, "", "", "");
	} else {
		// Here we could write some useful output back to the browser depending on
		// the path.
		printf("Received an invalid request: %s\n", ds->request_path().c_str());
		ds->Send("500 Sorry", true, "text/html", "",
			"<html><body>Sorry, not yet implemented</body></html>");
	}
}
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
	// Abort if the user specifies a port that is outside the allowed
	// range [1, 65535].
	Config *config = new Config("config.ini");
	string server_port;
	string port_key = "port";
	string ip;
	config->ReadConfig("ip",ip);

	config->ReadConfig(port_key,server_port);
	short FLAG_port = atoi(server_port.c_str());
	//short FLAG_port = 8888;
	if ((FLAG_port < 1) || (FLAG_port > 65535)) {
		printf("Error: %i is not a valid port.\n", FLAG_port);
		return -1;
	}
	/************1.创建socket，bind、listen***************/
	ListeningSocket listener;
	if (!listener.Create()) {
		printf("Failed to create server socket\n");
		return -1;
	} else if (!listener.Listen(FLAG_port)) {
		printf("Failed to listen on server socket\n");
		return -1;
	}

	printf("Server listening on port %i\n", FLAG_port);

	PeerChannel clients;
	typedef std::vector<DataSocket*> SocketArray;
	SocketArray sockets;
	bool quit = false;

	 while (!quit) {
		 fd_set socket_set;
		 FD_ZERO(&socket_set);
		 if (listener.valid())
			 FD_SET(listener.socket(), &socket_set);
		 for (SocketArray::iterator i = sockets.begin(); i != sockets.end(); ++i)
			 FD_SET((*i)->socket(), &socket_set);

		 struct timeval timeout = { 10, 0 };
		 if (select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout) == SOCKET_ERROR) {
			 printf("select failed\n");
			 break;
		 }

		 for (SocketArray::iterator i = sockets.begin(); i != sockets.end(); ++i) {
			 DataSocket* s = *i;
			 bool socket_done = true;
			 if (FD_ISSET(s->socket(), &socket_set)) {
				 if (s->OnDataAvailable(&socket_done) && s->request_received()) {
					 ChannelMember* member = clients.Lookup(s);
					 if (member || PeerChannel::IsPeerConnection(s)) {
						 if (!member) {
							 if (s->PathEquals("/sign_in")) {
								 clients.AddMember(s);
							 } else {
								 printf("No member found for: %s\n",
									 s->request_path().c_str());
								 s->Send("500 Error", true, "text/plain", "",
									 "Peer most likely gone.");
							 }
						 } else if (member->is_wait_request(s)) {
							 // no need to do anything.
							 socket_done = false;
						 } else {
							 ChannelMember* target = clients.IsTargetedRequest(s);
							 if (target) {
								 member->ForwardRequestToPeer(s, target);
							 } else if (s->PathEquals("/sign_out")) {
								 s->Send("200 OK", true, "text/plain", "", "");
							 } else {
								 printf("Couldn't find target for request: %s\n",
									 s->request_path().c_str());
								 s->Send("500 Error", true, "text/plain", "",
									 "Peer most likely gone.");
							 }
						 }
					 } else {
						 HandleBrowserRequest(s, &quit);
						 if (quit) {
							 printf("Quitting...\n");
							 FD_CLR(listener.socket(), &socket_set);
							 listener.Close();
							 clients.CloseAll();
						 }
					 }
				 }
			 } else {
				 socket_done = false;
			 }

			 if (socket_done) {
				 printf("Disconnecting socket\n");
				 clients.OnClosing(s);
				 assert(s->valid());  // Close must not have been called yet.
				 FD_CLR(s->socket(), &socket_set);
				 delete (*i);
				 i = sockets.erase(i);
				 if (i == sockets.end())
					 break;
			 }
		 }
		 clients.CheckForTimeout();

		 if (FD_ISSET(listener.socket(), &socket_set)) {
			 DataSocket* s = listener.Accept();
			 if (sockets.size() >= kMaxConnections) {
				 delete s;  // sorry, that's all we can take.
				 printf("Connection limit reached\n");
			 } else {
				 sockets.push_back(s);
				 printf("New connection...\n");
			 }
		 }
	 }

	for (SocketArray::iterator i = sockets.begin(); i != sockets.end(); ++i)
		delete (*i);
	sockets.clear();

	return 0;
}

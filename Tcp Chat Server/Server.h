#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <sstream>
#pragma comment (lib, "ws2_32.lib")

#define BUF_SIZE 4096

class Server
{
public:
	Server(const int port, const std::string ip);
	~Server();

public:
	void run();
	bool init();

private:
	void createListeningSocket();
	void Bind();
	void Listen();
	void initSet();
	void Clean();

private:
	std::string ipAddress;
	int port;
	sockaddr_in info; //struct to hold network info(i.e. port, ip)
	SOCKET listening;

private:
	fd_set masterSet; //file descriptor struct contains array of sockets and count
};
#include "Server.h"

Server::Server(const int port, const std::string ip)
	:port(port), ipAddress(ip)
{

}

Server::~Server()
{

}

void Server::run()
{
	createListeningSocket();

	while (true)
	{
		fd_set copy = masterSet;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept connection
				SOCKET client = accept(listening, (sockaddr*)&hint, 0);

				//add client to list
				FD_SET(client, &masterSet);

				std::string msg = "Welcome to the chat room\n\n\r";
				send(client, msg.c_str(), msg.size() + 1, 0);
				std::cout << "Client " << client << " connected to the server" << std::endl;
			}
			else //accept client msg
			{
				char buf[BUF_SIZE];
				ZeroMemory(buf, BUF_SIZE);

				int bytesRecv = recv(sock, buf, BUF_SIZE, 0);
				if (bytesRecv <= 0)
				{
					std::cout << "Client " << sock << " disconnected" << std::endl;
					closesocket(sock); //drop client
					FD_CLR(sock, &masterSet); //clear from set
				}
				else //send msg to other clients
				{
					for (int i = 0; i < masterSet.fd_count; i++)
					{
						SOCKET outSock = masterSet.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							std::stringstream ss;
								
							ss << "ClIENT #" << sock << ": " << buf << "\r";
							std::string sOut = ss.str();

							send(outSock, sOut.c_str(), sOut.size() + 1, 0); //send msg out to other clients
				
					     }
					}
				}
			}
		}
	}
	Clean();
}

bool Server::init()
{
	WSAData data;
	int start=WSAStartup(MAKEWORD(2, 2), &data);

	if (start != 0)
	{
		std::cout << "Winsock failed to init" << std::endl;
		return false;
	}
	return true;
}

void Server::createListeningSocket()
{
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cout << "Can't create socket" << std::endl;
		return;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	Bind();
}

void Server::Bind()
{
	bind(listening, (sockaddr*)&hint, sizeof(hint));
	Listen();
}

void Server::Listen()
{
	listen(listening, SOMAXCONN); //tell winsock, this socket is for listening
	initSet();
}

void Server::initSet()
{
	FD_ZERO(&masterSet);
	FD_SET(listening, &masterSet);
}

void Server::Clean()
{
	WSACleanup();
}

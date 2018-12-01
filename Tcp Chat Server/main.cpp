#include <iostream>
#include "Server.h"


int main()
{
	Server server(56000, "127.0.0.1");

	if (server.init())
	{
		server.run();
	}

	system("pause");
	return 0;
}
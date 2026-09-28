#include <iostream>
#include <irc/Server.hpp>
#include <irc/Message.hpp>
#include <queue>
#include <vector>
#include <poll.h>
#include <unistd.h>

//ayo Vic, open the listening socket here
Server::Server(int port, const std::string &pass): port(port), password(pass)
{


}

Server::~Server(void)
{
	for (std::vector<struct pollfd>::iterator i = poll_fds.begin(); i < poll_fds.end(); i++)
	{
		close((*i).fd);
	}
}

void	Server::InitServerLoop()
{
	while (true)
	{
		//poll
		//receive messages from users
		//process pending user messages
		//Accept new connections
	}
}

void	Server::ProcessMessages(void)
{
	for (std::vector<User>::iterator user = users.begin(); user < users.end(); user++)
	{
		if (!user->pending.empty())
			std::cout << user->nickname << ":" << std::endl;
		for (Message *message = &user->pending.front(); user->pending.empty(); message = &user->pending.front())
		{
			std::cout << (std::string) *message << std::endl;
			user->pending.pop();
		}
	}
}

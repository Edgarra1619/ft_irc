#include <irc/Server.hpp>
#include <irc/Message.hpp>

#include <iostream>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(const int port, const std::string& password) : password(password)
{
	listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (listen_fd == -1)
		throw std::runtime_error(std::string("socket() error: ") + strerror(errno));

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::runtime_error(std::string("bind() error: ") + strerror(errno));

	if (listen(listen_fd, listen_backlog) == -1)
		throw std::runtime_error(std::string("listen() error: ") + strerror(errno));

	std::cout << "server listening on port " << port << '\n';
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

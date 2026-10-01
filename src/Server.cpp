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
	const int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (listen_fd == -1)
		throw std::runtime_error(std::string("socket() error: ") + strerror(errno));

	const struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = { INADDR_ANY },
		.sin_zero = { 0 }
	};

	if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::runtime_error(std::string("bind() error: ") + strerror(errno));

	static const int listen_backlog = 16;

	if (listen(listen_fd, listen_backlog) == -1)
		throw std::runtime_error(std::string("listen() error: ") + strerror(errno));

	const struct pollfd listen_poll_fd = {
		.fd = listen_fd,
		.events = POLLIN,
		.revents = 0
	};

	poll_fds.push_back(listen_poll_fd);

	std::cout << "server listening on port " << port << '\n';
}

Server::~Server(void)
{
	for (std::vector<struct pollfd>::iterator i = poll_fds.begin(); i != poll_fds.end(); ++i)
	{
		close((*i).fd);
	}
}

void Server::Loop(void)
{
	while (true)
	{
		poll(poll_fds.data(), poll_fds.size(), 1);

		CloseConnections();

		//receive messages from users
		//process pending user messages

		if (poll_fds.front().revents & POLLIN)
			HandleConnection();
	}
}

void Server::ProcessMessages(void)
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

void Server::HandleConnection(void)
{
	const int user_fd = accept(poll_fds.front().fd, NULL, NULL);
	if (user_fd == -1)
		throw std::runtime_error(std::string("accept() error: ") + strerror(errno));

	const struct pollfd user_poll_fd = {
		.fd = user_fd,
		.events = POLLIN | POLLRDHUP,
		.revents = 0
	};

	poll_fds.push_back(user_poll_fd);

	const User user(user_fd);

	users.push_back(user);

	std::cout << "user connected\n";
}

void Server::CloseConnections(void)
{
	std::vector<struct pollfd>::iterator poll_fd = ++poll_fds.begin();
	std::vector<User>::iterator user = users.begin();

	while (poll_fd != poll_fds.end())
	{
		if (poll_fd->revents & (POLLERR | POLLHUP | POLLNVAL | POLLRDHUP))
		{
			close(poll_fd->fd);
			poll_fd = poll_fds.erase(poll_fd);
			user = users.erase(user);

			std::cout << "closed user connection\n";

			continue;
		}
		++poll_fd;
		++user;
	}
}

#include <irc/User.hpp>

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <poll.h>

User::User(int fd) : fd(fd) {}

bool User::operator<(const User& rhs)
{
	return (nickname < rhs.nickname);
}

void User::Loop(const short events)
{
	if (events & POLLIN)
		ReceiveData();
}

void User::ReceiveData(void)
{
	static const ssize_t buffer_size = 1024;
	static char buffer[buffer_size];

	while (true)
	{
		const ssize_t count = recv(fd, buffer, buffer_size, 0);
		if (count == -1)
			throw std::runtime_error(std::string("recv() error: ") + strerror(errno));

		received.append(buffer, count);

		if (count < buffer_size)
			break;
	}
}

void User::SendTo(const std::string& msg)
{
	(void)msg;
}

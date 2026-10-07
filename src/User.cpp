#include <irc/User.hpp>

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <poll.h>

User::User(const int fd) : fd(fd) {}

bool User::operator<(const User& rhs) const
{
	return (nickname < rhs.nickname);
}

void User::Loop(const short events)
{
	if (events & POLLIN)
	{
		ReceiveData();
		SplitMessages();
	}
}

void User::ReceiveData(void)
{
	static char buffer[Message::size_max];

	while (true)
	{
		const ssize_t count = recv(fd, buffer, Message::size_max, 0);
		if (count == -1)
			throw std::runtime_error(std::string("recv() error: ") + strerror(errno));

		received.append(buffer, count);

		if (count < static_cast<ssize_t>(Message::size_max))
			return;
	}
}

void User::SplitMessages(void)
{
	while (true)
	{
		const std::string::size_type pos = received.find(Message::delim);
		if (pos == std::string::npos)
			return;

		std::string message;
		if (pos + Message::delim.size() <= Message::size_max)
			message = received.substr(0, pos + Message::delim.size());
		else
			message = received.substr(0, Message::size_max - Message::delim.size()) + Message::delim;
		pending.push(Message(message, *this));

		received.erase(0, pos + Message::delim.size());

		std::cout << "received message from user: " << message;
	}
}

void User::SendData(const std::string& data) const
{
	if (send(fd, data.c_str(), data.size(), 0) == -1)
		throw std::runtime_error(std::string("send() error: ") + strerror(errno));
}

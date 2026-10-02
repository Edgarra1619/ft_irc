#include <irc/Server.hpp>
#include <irc/Message.hpp>

#include <iostream>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(const int port, const std::string& password) : prefix("example.com"), password(password)
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
	poll(poll_fds.data(), poll_fds.size(), 0);

	CloseConnections();
	for (std::size_t i = 0; i < users.size(); i++)
	{
		if (poll_fds[i + 1].revents & POLLIN)
			users[i].Receive();
	}
	ProcessMessages();
	//receive messages from users
	//process pending user messages

	if (poll_fds.front().revents & POLLIN)
		HandleConnection();
}

void Server::ProcessMessages(void)
{
	for (std::vector<User>::iterator user = users.begin(); user != users.end(); user++)
	{
		for (;!user->pending.empty(); user->pending.pop())
		{
			Message *message = &user->pending.front();
			std::cout << user->nickname << " sending:\n";
			std::cout << (std::string) *message << std::flush;
			if (message->command == "PING")
			{
				message->command = "PONG";
				message->prefix = prefix;
				user->SendTo(*message);
			}
			else if(message->command == "PASS")
			{
				//ERR_NEEDMOREPARAMS
				//ERR_ALREADYREGISTRED
			}
			else if(message->command == "QUIT")
			{
			}
			else if(message->command == "NICK")
			{
				//ERR_NONICKNAMEGIVEN
				//ERR_ERRONEUSNICKNAME
				//ERR_NICKNAMEINUSE
				//ERR_NICKCOLLISION
			}
			else if(message->command == "USER")
			{
				//ERR_NEEDMOREPARAMS
				//ERR_ALREADYREGISTRED
			}
			else if(message->command == "PRIVMSG")
			{
				//ERR_NOSUCHNICK
				//ERR_CANNOTSENDTOCHAN
				//ERR_TOOMANYTARGETS
				//ERR_NORECIPIENT
				//ERR_NOTEXTTOSEND
				//ERR_NOTOPLEVEL
				//ERR_WILDTOPLEVEL
				//ERR_NOTREGISTERED
			}
			else if(message->command == "JOIN")
			{
				//RPL_NOTOPIC
				//RPL_TOPIC
				//RPL_NAMREPLY
				//ERR_NOSUCHCHANNEL
				//ERR_TOOMANYCHANNELS
				//ERR_NEEDMOREPARAMS
				//ERR_NOTREGISTERED
				//ERR_CHANNELISFULL
				//ERR_INVITEONLYCHAN
				//ERR_BANNEDFROMCHAN
				//ERR_BADCHANNELKEY
			}
			else if(message->command == "PART")
			{
				//ERR_NOSUCHCHANNEL
				//ERR_NOTONCHANNEL
				//ERR_NEEDMOREPARAMS
				//ERR_NOTREGISTERED
			}
			else if(message->command == "KICK")
			{
				//ERR_NOSUCHCHANNEL
				//ERR_NOTONCHANNEL
				//ERR_NEEDMOREPARAMS
				//ERR_NOTREGISTERED
				//ERR_CHANOPRIVSNEEDED
			}
			else if(message->command == "INVITE")
			{
				//RPL_INVITING
				//ERR_NOSUCHNICK
				//ERR_NOTONCHANNEL
				//ERR_USERONCHANNEL
				//ERR_NEEDMOREPARAMS
				//ERR_NOTREGISTERED
				//ERR_CHANOPRIVSNEEDED
			}
			else if(message->command == "TOPIC")
			{
				//RPL_NOTOPIC
				//RPL_TOPIC
				//ERR_NEEDMOREPARAMS
				//ERR_NOTONCHANNEL
				//ERR_NOTREGISTERED
				//ERR_CHANOPRIVSNEEDED
			}
			else if(message->command == "MODE")
			{
				//only implement channel mode
				//
				//RPL_CHANNELMODEIS
				//RPL_BANLIST
				//RPL_ENDOFBANLIST
				//ERR_NOSUCHNICK
				//ERR_NOTONCHANNEL
				//ERR_NEEDMOREPARAMS
				//ERR_CHANOPRIVSNEEDED
				//ERR_KEYSET
				//ERR_UNKNOWNMODE
				//ERR_NOSUCHCHANNEL
				//ERR_NOTREGISTERED
			}
			else
			{
				//ERR_UNKNOWNCOMMAND
			}
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

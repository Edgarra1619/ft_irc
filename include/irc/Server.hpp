#ifndef SERVER_HPP
#define SERVER_HPP

#include <irc/User.hpp>

#include <string>
#include <vector>

#include <poll.h>

class Server
{
private:
	static const int listen_backlog = 16;

public:
	int listen_fd;
	std::string password;
	std::vector<User> users;
	std::vector<struct pollfd> poll_fds;

	Server(int port, const std::string& password);
	~Server(void);

	void InitServerLoop(void);
	void ProcessMessages(void);
};

#endif

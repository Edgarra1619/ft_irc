#ifndef SERVER_HPP
#define SERVER_HPP

#include <irc/User.hpp>

#include <string>
#include <vector>

#include <poll.h>

class Server
{
public:
	std::string password;
	std::vector<User> users;
	std::vector<struct pollfd> poll_fds;

	Server(int port, const std::string& password);
	~Server(void);

	void Loop(void);
	void ProcessMessages(void);
	void HandleConnection(void);
	void CloseConnections(void);
};

#endif

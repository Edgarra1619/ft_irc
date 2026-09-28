#ifndef SERVER_HPP
# define SERVER_HPP
# include <irc/User.hpp>
# include <irc/Channel.hpp>
# include <vector>
# include <poll.h>

class Server
{
public:
	int	port;
	std::string	password;
	std::vector<User> users;
	std::vector<struct pollfd> poll_fds;

	void	InitServerLoop(void);
	void	ProcessMessages(void);
	Server(int port, const std::string &pass);
	~Server(void);
};


#endif

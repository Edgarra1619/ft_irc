#ifndef USER_HPP
# define USER_HPP
# include <list>
# include <string>
# include <queue>
# include <irc/Message.hpp>

struct Message;
class Channel;

class User
{
private:
	int fd;
	std::string received;
public:
	std::string nickname;
	std::string username;
	bool serverOperator;

	std::queue<Message> pending;
	std::list<Channel*> channels;

	User(int fd);
	void SendTo(const std::string&);
	bool operator <(const User& rhs);

};

#endif

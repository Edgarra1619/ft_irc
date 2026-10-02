#ifndef USER_HPP
#define USER_HPP

#include <irc/Message.hpp>
#include <irc/Channel.hpp>

#include <string>
#include <list>
#include <queue>

class User
{
private:
	int fd;
	std::string received;

public:
	std::string nickname;
	std::string username;
	bool server_operator;

	std::queue<Message> pending;
	std::list<Channel*> channels;

	User(int fd);

	bool operator<(const User& rhs);

	void Loop(short events);
	void ReceiveData(void);
	void SplitMessages(void);
	void SendTo(const std::string&);
};

#endif

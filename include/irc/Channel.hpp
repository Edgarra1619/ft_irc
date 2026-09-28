#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <map>
#include <set>
#include <string>

class User;
struct Message;

class Channel
{
private:
	std::map<User*, bool> users;
	std::set<const User*> invitees;
public:
	std::string name;
	Channel(std::string);
	char mode;
	void Join(User&);
	void Part(User&);
	void Invite(const User&);
	void SendTo(const Message&);
};
#endif

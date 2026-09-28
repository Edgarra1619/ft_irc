#include <irc/Channel.hpp>
#include <irc/User.hpp>
#include <irc/Message.hpp>
#include <map>
#include <string>
#include <utility>

Channel::Channel(std::string name): name(name){}

//mode invite not implemented
void	Channel::Join(User& user)
{
	users.insert(std::pair<User*, bool>(&user, 0));
}

//for now, returns silently
void	Channel::Part(User& user)
{
	users.erase(&user);
}

//return silently if already invited
void	Channel::Invite(const User& user)
{
	invitees.insert(&user);
}

void	Channel::SendTo(const Message& msg)
{
	for (std::map<User*, bool>::const_iterator i = users.begin(); i != users.end(); i++)
	{
		i->first->SendTo((std::string) msg);
	}

}

#include <irc/User.hpp>

bool User::operator <(const User& rhs)
{
	return (nickname < rhs.nickname);
}

User::User(int fd) : fd(fd) {}

void	User::SendTo(const std::string& msg)
{
	(void) msg;

}

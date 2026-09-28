#ifndef MESSAGE_HPP
# define MESSAGE_HPP
#include <string>
#include <vector>

class User;

struct Message
{
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
	Message(std::string, const User&);
	Message(std::string str);
	operator std::string() const;
};
#endif

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

class User;

struct Message
{
	static const size_t size_max = 1024;
	static const std::string delim;

	std::string prefix;
	std::string command;
	std::vector<std::string> params;

	Message(std::string, const User&);
	Message(std::string str);

	operator std::string() const;
};
#endif

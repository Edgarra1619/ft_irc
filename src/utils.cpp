#include <cctype>
#include <cstdlib>
#include <irc/utils.hpp>
#include <stdexcept>

//atoi that throws an exception when the str isn't a number
int parse_num(const std::string &str, const std::string &name = "str")
{
	for (std::string::const_iterator i = str.begin(); i < str.end(); i++)
	{
		if (!std::isdigit(*i))
			throw(std::invalid_argument(name + " isn't number"));
	}
	return (std::atoi(str.c_str()));
}

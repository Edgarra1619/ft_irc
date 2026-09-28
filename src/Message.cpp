#include <irc/Message.hpp>
#include <irc/User.hpp>
#include <string>
#include <vector>


//separate string into message parts
Message::Message(std::string str, const User& sender)
{
	if (str[0] != ':')
		prefix = ":" + sender.nickname;
	else
	{
		prefix = str.substr(0, str.find_first_of(' '));
		str = str.substr(str.find_first_not_of(' ', str.find_first_of(' ')));
	}
	command = str.substr(0, str.find_first_of(' '));
	while (str.find_first_of(' ') != std::string::npos)
	{
		str = str.substr(str.find_first_not_of(' ', str.find_first_of(' ')));
		params.push_back(str.substr(0, str.find_first_of(' ')));
	}
}

Message::Message(std::string str)
{
	if (str[0] == ':')
	{
		prefix = str.substr(0, str.find_first_of(' '));
		str = str.substr(str.find_first_not_of(' ', str.find_first_of(' ')));
	}
	command = str.substr(0, str.find_first_of(' '));
	while (str.find_first_not_of(' ', str.find_first_of(' ')) != std::string::npos)
	{
		str = str.substr(str.find_first_not_of(' ', str.find_first_of(' ')));
		params.push_back(str.substr(0, str.find_first_of(' ')));
	}
}

Message::operator std::string() const
{
	std::string	str;

	str = prefix + command;
	for (std::vector<std::string>::const_iterator i = params.begin(); i < params.end(); i++)
	{
		str += " " + *i;
	}
	return (str);
}

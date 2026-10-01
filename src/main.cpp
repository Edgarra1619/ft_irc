#include <irc/Server.hpp>

#include <iostream>

int main(void)
{
	try
	{
		Server server(2000, "password");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}

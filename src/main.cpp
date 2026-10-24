#include <irc/Server.hpp>
#include <irc/utils.hpp>

#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Expected 2 arguments, got " << argc - 1 << std::endl;
		return (1);
	}
	try
	{
		Server server(parse_num(argv[1], "port"), argv[2]);
		while (true)
			server.Loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}

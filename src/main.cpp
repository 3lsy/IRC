#include "IRC.hpp"

int main(int ac, char **av)
{
	int	port;

	if (ac == 3)
	{
		port = atoi(av[1]);
		if (!IRC::valid_port(port))
			return (1);
		std::signal(SIGINT, IRC::signal_handler);
        IRC* irc = IRC::getInstance(port, av[2]);
		irc->serve();
		return (0);
	}
	std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 11:57:58 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/01 17:56:14 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
/*
int main() {
    int port = 6667;  // Default IRC port
    std::string password = "password";
    IRC server(port, password);
    server.serve();
    return 0;
}
*/

int main(int ac, char **av)
{
	int	port;

	if (ac == 3)
	{
		port = atoi(av[1]);
		if (!IRC::valid_port(port))
			return (1);
		IRC irc(port, av[2]);
		irc.serve();
		return (0);
	}
	std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
	return (1);
}

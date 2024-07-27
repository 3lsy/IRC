/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 11:57:58 by echavez-          #+#    #+#             */
/*   Updated: 2024/07/27 12:49:02 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
		std::cout << "Port: " << av[1] << std::endl;
		std::cout << "Password: " << av[2] << std::endl;
		return (0);
	}
	std::cout << "Usage: ./irc <port> <password>" << std::endl;
	return (1);
}
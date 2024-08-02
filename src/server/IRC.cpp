/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:22:11 by echavez-          #+#    #+#             */
/*   Updated: 2024/07/29 15:57:57 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

bool	IRC::valid_port(int port) {
	if (port < 1024 || port > 65535)
	{
		std::cerr << "Error: Invalid port" << std::endl;
		std::cerr << "Port must be a number between 1024 and 65535" << std::endl;
		return (false);
	}
	return (true);
}

IRC::IRC() {
    // Constructor implementation
}

IRC::IRC(int port, std::string password): _port(port), _password(password) {
	this->_create_socket();
	this->_bind_socket();
	if (listen(this->_socket_fd, 100) < 0)
	{
		std::cerr << "Error: Unable to listen on socket" << std::endl;
		exit(1);
	}
}

IRC::~IRC() {
    // Destructor implementation
}

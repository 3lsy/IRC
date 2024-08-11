/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:22:11 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 17:12:01 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

IRC* IRC::instance = NULL;

bool	IRC::valid_port(int port) {
	if (port < 1024 || port > 65535)
	{
		std::cerr << RED << "SERVER: Error: Invalid port" << RESET << std::endl;
		std::cerr << "Port must be a number between 1024 and 65535" << std::endl;
		return (false);
	}
	return (true);
}

void	IRC::signal_handler(int signum __attribute__((unused))) {
	std::cout << BLUE << "SERVER: Stopping server" << RESET << std::endl;
	if (instance) {
        instance->stop_server();
		delete instance;
    }
}

IRC::IRC(int port, std::string password):
	_port(port), _password(password), _n_clients(0), _running(true)
{
	int	opt;

	this->_create_socket();
	this->_bind_socket();
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << RED << "SERVER: Error: Unable to set socket options" << RESET << std::endl;
		exit(1);
	}
	if (listen(this->_socket_fd, 100) < 0)
	{
		std::cerr << RED << "SERVER: Error: Unable to listen on socket" << RESET << std::endl;
		exit(1);
	}
}

IRC::~IRC(void) {
	std::cout << BLUE << "SERVER: IRC server shutting down" << RESET << std::endl;

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		delete it->second;
	}
	_clients.clear();
	if (this->_socket_fd >= 0) {
		close(this->_socket_fd);
		std::cout << BLUE << "SERVER: Server socket closed" << RESET << std::endl;
	}
}

IRC* IRC::getInstance(int port, std::string password) {
    if (instance == NULL) {
        instance = new IRC(port, password);
    }
    return instance;
}

std::string IRC::getPassword(void) const {
	return this->_password;
}

int IRC::getPort(void) const {
	return this->_port;
}
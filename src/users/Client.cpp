/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:44:36 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/18 21:47:20 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {
}

Client::Client(int server_fd) : _password(false), _servername(""), _mode(""), _ip(""), nickname(""),
                                username(""), realname(""), hostname(""), logged_in(false)
{
	this->socket_len = sizeof(this->socket_addr);
	this->fd = accept(server_fd, (struct sockaddr *)&this->socket_addr, &this->socket_len);
	if (this->fd < 0)
	{
		std::cerr << RED << "SERVER: Error: Unable to accept connection" << RESET << std::endl;
		return ;
	}
	std::cout << BLUE << "SERVER: New connection from " << inet_ntoa(this->socket_addr.sin_addr) << RESET << std::endl;
}

Client::~Client(void) {
	if (this->fd >= 0) {
        close(this->fd);
        std::cout << BLUE << "SERVER: Client socket closed" << RESET << std::endl;
    }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:44:36 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 15:09:46 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {
}

Client::Client(int server_fd) : _password(false), _nickname(""), _username(""), _realname(""),
                            _hostname(""), _servername(""), _mode(""), _ip(""), logged_in(false)
{
	this->socket_len = sizeof(this->socket_addr);
	this->fd = accept(server_fd, (struct sockaddr *)&this->socket_addr, &this->socket_len);
	if (this->fd < 0)
	{
		std::cerr << "Error: Unable to accept connection" << std::endl;
		return ;
	}
	std::cout << "New connection from " << inet_ntoa(this->socket_addr.sin_addr) << std::endl;
}

Client::~Client(void) {
	if (this->fd >= 0) {
        close(this->fd);
        std::cout << "Client socket closed" << std::endl;
    }
}

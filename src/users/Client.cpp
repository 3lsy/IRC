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

Client::Client(int fd, struct sockaddr_in addr) : _password(false), _servername(""), _mode(""), _ip(""), fd(fd), nickname(""),
                                username(""), realname(""), hostname(""), logged_in(false), socket_addr(addr)
{
	this->socket_len = sizeof(this->socket_addr);
}

Client::~Client(void) {
	if (this->fd >= 0) {
        close(this->fd);
        std::cout << BLUE << "SERVER: Client socket closed" << RESET << std::endl;
    }
}

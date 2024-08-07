/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_socket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:24:44 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/07 14:51:02 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

/**
 * @brief Creates a socket for the IRC server.
 * 
 * Creates a socket using the `socket()` system call and stores the file descriptor
 * in the `_socket_fd` attribute. Outputs an error message and exits if the socket
 * creation fails.
 */
void	IRC::_create_socket(void)
{
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd < 0)
	{
		std::cerr << "Error: Unable to create socket" << std::endl;
		exit(1);
	}
}

/**
 * @brief Binds the socket to the specified port and address.
 * 
 * Initializes the socket address structure and binds the socket to all available
 * network interfaces using the specified port. Outputs an error message and exits
 * if the binding fails.
 * 
 * @note Uses `INADDR_ANY` to listen on all network interfaces and `htons()` to
 *       convert the port number to network byte order.
 */
void	IRC::_bind_socket(void)
{
    int optval = 1;
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 ||
        setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
        std::cerr << "Error: Unable to set socket options" << std::endl;
        close(this->_socket_fd);
        exit(1);
    }
	std::memset(&this->_serv_addr, 0, sizeof(this->_serv_addr));
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htons(this->_port);
	this->_max_fd = this->_socket_fd;
	if (bind(this->_socket_fd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) < 0)
	{
		std::cerr << "Error: Unable to bind socket" << std::endl;
		exit(1);
	}
}

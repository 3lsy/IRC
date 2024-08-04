/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_serve.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:58:50 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/04 18:40:44 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

/**
 * @brief Initialize the fd sets
 * 
 * This function initializes the read and write sets with the master set
 * and calls select to wait for events
 * 
 * @note The write set is to know if the server can write to the client
 */
void	IRC::_init_fd_sets(void)
{
	int n_fds;

	memcpy(&this->_read_set, &this->_master_set, sizeof(this->_master_set));
	memcpy(&this->_write_set, &this->_master_set, sizeof(this->_master_set));
	// n_fds = select(this->_max_fd + 1, &this->_read_set, &this->_write_set, NULL, NULL);
	n_fds = select(this->_max_fd + 1, &this->_read_set, NULL, NULL, NULL); // to debug only read
	if (n_fds < 0 && errno != EINTR)
	{
		std::cerr << "Error: select error: " << strerror(errno) << std::endl;
		exit(1);
	}
}

/**
 * @brief Search for events in the read
 * 
 * This function iterates through all file descriptors to find the one that triggered the event.
 * If the fd is the socket, it means there's a new incoming connection request.
 * If the fd is a client, it reads from the client.
 * 
 * @note After the server reads from client, it will treat the request
 */
void	IRC::_event_search(void)
{
	for (int i_fd = 0; i_fd <= this->_max_fd; ++i_fd)
	{
		if (FD_ISSET(i_fd, &this->_read_set))
		{
			if (i_fd == this->_socket_fd)
				this->_new_connection();
			else
				this->_read_from_client(i_fd);
		}
	}
}

/**
 * @brief Main loop for the server
 */
void IRC::serve(void) {
    std::cout << "IRC server started on port " << _port << std::endl;
	FD_ZERO(&this->_master_set);
	FD_SET(this->_socket_fd, &this->_master_set);
	this->_max_fd = this->_socket_fd;

	while (this->_running) {
		this->_init_fd_sets();
		if (!this->_running)
			break;
		this->_event_search();
	}
}

/**
 * @brief Stop the server
 */
void	IRC::stop_server(void) {
	this->_running = false;
}

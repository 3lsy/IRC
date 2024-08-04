/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:28:08 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/04 22:11:27 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void	IRC::_new_connection(void)
{
	// Client new client(this->_socket_fd);
	Client *client = new Client(this->_socket_fd);

	if (client->fd < 0) {
		delete client;
		return ;
	}
	FD_SET(client->fd, &this->_master_set);
	if (client->fd > this->_max_fd) {
		this->_max_fd = client->fd;
	}
	this->_clients.push_back(client);
	this->_n_clients++;
}

void	IRC::_read_from_client(int fd)
{
	this->_bytes_read = recv(fd, this->_buffer, sizeof(this->_buffer), 0);
	if (this->_bytes_read <= 0)
	{
		if (this->_bytes_read == 0)
			std::cout << "Client disconnected" << std::endl;
		else
			std::cerr << "Error: Unable to read from client" << std::endl;
		close(fd);
		FD_CLR(fd, &this->_master_set);
		this->_n_clients--;
		this->_remove_client(fd);
	}
	else
	{
		this->_buffer[this->_bytes_read] = '\0';
		std::cout << "Received: " << this->_buffer << std::endl;
		send(fd, this->_buffer, this->_bytes_read, 0);
		send(fd, ":server.hostname 001 echavez- :Welcome to the IRC Network echavez-!echavez-@echavez-", 84, 0);
		// for (int i_fd = 0; i_fd <= this->_max_fd; ++i_fd)
		// {
		// 	if (FD_ISSET(i_fd, &this->_write_set))
		// 	{
		// 		send(i_fd, ":server.hostname 001 echavez- :Welcome to the IRC Network echavez-!echavez-@echavez-", 84, 0);
		// 	}
		// }
	}
}

void	IRC::_remove_client(int fd)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if ((*it)->fd == fd)
		{
			delete *it;
			it = this->_clients.erase(it);
			break;
		}
		else
			++it;
	}
}
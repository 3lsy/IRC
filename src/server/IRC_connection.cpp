/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:28:08 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 14:28:05 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Client.hpp"

void IRC::_new_connection(void)
{
    Client *client = new Client(this->_socket_fd);

    if (client->fd < 0) {
        delete client;
        return ;
    }
    FD_SET(client->fd, &this->_master_set);
    if (client->fd > this->_max_fd) {
        this->_max_fd = client->fd;
    }
    // Insert the client into the map using the fd as the key
    this->_clients[client->fd] = client;
    this->_n_clients++;
    std::cout << "New client connected with fd: " << client->fd << std::endl << std::endl;
}


/**
 * @brief This function reads from the client and sends a response back
 * 
 * @todo Correct: errno = "Connection reset by peer" when client disconnects (recv returns -1)
 */
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
		this->remove_client(fd);
	}
	else
	{
		this->_buffer[this->_bytes_read] = '\0';
		std::cout << "Received: " << this->_buffer << std::endl;
		this->_clients[fd]->command_handler(this->_buffer);
	}
}

/**
 * @brief This function removes a client from the map and deletes the object
 * 
 * @param fd The file descriptor of the client to remove
 */
void IRC::remove_client(int fd)
{
    // Find the client in the map
    std::map<int, Client *>::iterator it = this->_clients.find(fd);
    if (it != this->_clients.end())
    {
        delete it->second;
        this->_clients.erase(it);
        this->_n_clients--;
        std::cout << "Client with fd: " << fd << " has been removed." << std::endl;
    }
    else
        std::cout << "Client with fd: " << fd << " not found!" << std::endl;
}
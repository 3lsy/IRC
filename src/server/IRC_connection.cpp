/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:28:08 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 18:09:16 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Client.hpp"

void IRC::_new_connection(void)
{
    Client *client = new Client(this->_socket_fd);

    if (client->fd < 0) {
        delete client;
        client = NULL;
        return ;
    }
    FD_SET(client->fd, &this->_master_set);
    if (client->fd > this->_max_fd) {
        this->_max_fd = client->fd;
    }
    // Insert the client into the map using the fd as the key
    this->_clients[client->fd] = client;
    this->_n_clients++;
    std::cout << BLUE << "SERVER: New client connected with fd: " << client->fd << RESET << std::endl;
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
			std::cout << BLUE << "SERVER: Client disconnected" << RESET << std::endl;
		else
			std::cerr << RED << "SERVER: Error: Unable to read from client" << RESET << std::endl;
		this->remove_client(fd);
	}
	else
	{
		this->_buffer[this->_bytes_read] = '\0';
		std::cout << GREEN << "CLIENT: " << this->_buffer << RESET << std::endl;
        if (!this->_clients[fd]->logged_in)
        {
            if (this->_clients[fd]->login(this->_buffer) == false)
            {
                this->remove_client(fd);
                return ;   
            }
            if (this->_clients[fd]->logged_in)
                this->_nicknames[this->_clients[fd]->nickname] = fd;
        }
	}
}

/**
 * @brief This function removes a client from the map and deletes the object
 * 
 * @param fd The file descriptor of the client to remove
 */
void IRC::remove_client(int fd)
{
    // if nickname in map, erase it
    if (this->_nicknames.find(this->_clients[fd]->nickname) != this->_nicknames.end())
        this->_nicknames.erase(this->_clients[fd]->nickname);
    // Find the client in the map
    std::map<int, Client *>::iterator it = this->_clients.find(fd);
    if (it != this->_clients.end())
    {
        FD_CLR(fd, &this->_master_set);
        delete it->second;
        this->_clients.erase(it);
        this->_n_clients--;
        std::cout << "SERVER: Client with fd: " << fd << " has been removed." << std::endl;
    }
    else
        std::cout << "SERVER: Client with fd: " << fd << " not found!" << std::endl;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_connection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:28:08 by echavez-          #+#    #+#             */
/*   Updated: 2024/11/10 18:09:09 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Client.hpp"

void IRC::_new_connection(void)
{
    struct sockaddr_in	client_addr;
    socklen_t			addr_len = sizeof(client_addr);
    int client_fd = accept(this->_socket_fd, (struct sockaddr *)&client_addr, &addr_len);
    
    if (client_fd < 0) {
        std::cerr << RED << "SERVER: Error: Unable to accept connection: "
                  << strerror(errno) << RESET << std::endl;
        return;
    }
    std::cout << BLUE << "SERVER: New connection from " 
              << inet_ntoa(client_addr.sin_addr) << RESET << std::endl;

    Client* client = new Client(client_fd, client_addr);
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
 * @param fd The file descriptor of the client
 * 
 * @note Client format messages: :<nickname>!<username>@<hostname> PRIVMSG <target> :<message>
 * @note Server format messages: :<servername> 433 * <nickname> :Nickname is already in use
 */
void    IRC::_read_client_message(int fd)
{
    if (!this->_clients[fd]->logged_in)
    {
        if (this->_clients[fd]->login(this->_buffer, this->_nicknames, this->_channels) == false)
        {
            return ;
        }
        if (this->_clients[fd]->logged_in)
            this->_nicknames[this->_clients[fd]->nickname] = fd;
    }
    else
        this->_interaction(this->_buffer, fd);  
}

/**
 * @brief This function reads from the client and sends a response back
 * 
 */
void	IRC::_read_from_client(int fd)
{
    char temp_buffer[1024 + 1];
	this->_bytes_read = recv(fd, temp_buffer, sizeof(temp_buffer), 0);
	if (this->_bytes_read <= 0)
	{
		if (this->_bytes_read == 0)
			std::cout << BLUE << "SERVER: Client disconnected" << RESET << std::endl;
		else
			std::cerr << RED << "SERVER: Error: Unable to read from client" << RESET << std::endl;
		this->remove_client(fd);
        return;
	}
    temp_buffer[this->_bytes_read] = '\0';
    Client *client = this->_clients[fd];
    if (client->input_buffer.empty())
        client->input_buffer = temp_buffer;
    else
        client->input_buffer += temp_buffer;

    size_t pos;
    while ((pos = client->input_buffer.find('\n')) != std::string::npos)
    {
        std::string line = client->input_buffer.substr(0, pos);
        client->input_buffer.erase(0, pos + 1); // Remove `\n`
        if (!line.empty())
        {
            if (line[line.size() - 1] == '\r') // Remove `\r` if present
                line.erase(line.size() - 1);
            strcpy(this->_buffer, line.c_str());
            
            std::cout << GREEN << "CLIENT{";
            if (!this->_clients[fd]->nickname.empty())
                std::cout << this->_clients[fd]->nickname;
            else
                std::cout << fd;
            std::cout << "}: " << this->_buffer << RESET << std::endl;

            this->_read_client_message(fd);
            // protect if client might have been deleted
            if (this->_clients.find(fd) == this->_clients.end())
                return;
            client = this->_clients[fd]; // Re-fetching pointer
        }
    }
}

/**
 * @brief This function removes a client from the map and deletes the object
 * 
 * @param fd The file descriptor of the client to remove
 */
void IRC::remove_client(int fd, bool remove_nickname)
{
    // if nickname in map, erase it
    if (remove_nickname && this->_nicknames.find(this->_clients[fd]->nickname) != this->_nicknames.end())
        this->_nicknames.erase(this->_clients[fd]->nickname);
    // Find the client in the map
    std::map<int, Client *>::iterator it = this->_clients.find(fd);
    if (it != this->_clients.end())
    {
        FD_CLR(fd, &this->_master_set);
        delete it->second;
        this->_clients.erase(it);
        this->_n_clients--;
        std::cout << BLUE << "SERVER: Client with fd: " << fd << " has been removed." << RESET << std::endl;
    }
    else
        std::cout << BLUE << "SERVER: Client with fd: " << fd << " not found!" << RESET << std::endl;
}
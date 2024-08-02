/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_serve.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 14:58:50 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/01 17:44:40 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

// test
void IRC::serve() {
    std::cout << "IRC server started on port " << _port << std::endl;
    this->_accept_connections();
}

void IRC::_accept_connections() {
    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    FD_SET(this->_socket_fd, &master_set);
    this->_max_fd = this->_socket_fd;

    while (true) {
        memcpy(&working_set, &master_set, sizeof(master_set));
        std::cout << "Waiting for connections..." << std::endl;
        int activity = select(this->_max_fd + 1, &working_set, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR) {
            std::cerr << "Error: select error" << std::endl;
            exit(1);
        }

        for (int i = 0; i <= this->_max_fd; ++i) {
            if (FD_ISSET(i, &working_set)) {
                if (i == this->_socket_fd) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(this->_socket_fd, (struct sockaddr *)&client_addr, &client_len);

                    if (client_fd < 0) {
                        std::cerr << "Error: Unable to accept connection" << std::endl;
                        continue;
                    }
                    std::cout << "Client connected" << std::endl;
                    FD_SET(client_fd, &master_set);
                    if (client_fd > this->_max_fd) {
                        this->_max_fd = client_fd;
                    }
                } else {
                    char buffer[1024];
                    int bytes_read = recv(i, buffer, sizeof(buffer), 0);

                    if (bytes_read <= 0) {
                        if (bytes_read == 0) {
                            std::cout << "Client disconnected" << std::endl;
                        } else {
                            std::cerr << "Error: recv error" << std::endl;
                        }
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        buffer[bytes_read] = '\0';
                        std::cout << "Received: " << buffer << std::endl;
                        send(i, buffer, bytes_read, 0);
                    }
                }
            }
        }
    }
}

/*
// infinite loop for the server
void	IRC::serve()
{
	FD_ZERO(&this->_master_set);
	FD_SET(this->_socket_fd, &this->_master_set);
	while (1)
	{
		this->_read_set = this->_master_set;
		this->_write_set = this->_master_set;
		if (select(this->_max_fd + 1, &this->_read_set, &this->_write_set, NULL, NULL) < 0)
			continue;
		this->_event_search(this->_max_fd);
	}
}

// This function is to iterate through all file descriptors to find the one that triggered the event
// if the fd is the socket, it means there's a new incoming connection request
void	IRC::_event_search(int max_fd)
{
	for (int fd = 0; fd <= max_fd; fd++)
	{
		if (FD_ISSET(fd, &this->_read_set))
		{
			if (fd == this->_socket_fd)
				if (this->_accept_client() < 0)
					continue;
			else
				this->_read_from_client(fd);
		}
	}
}

// accept a new client connection
//
//TODO: Implement Channels so that the server knows which channel the user joined
//
int IRC::_accept_client()
{
	this->_client_fd = accept(this->_socket_fd, (struct sockaddr *)&this->_serv_addr, (socklen_t *)&this->_serv_addr);
	if (this->_client_fd < 0)
		return (-1);
	if (this->_client_fd > this->_max_fd)
		this->_max_fd = this->_client_fd;
	//client count
	this->_n_clients++;
	FD_SET(this->_client_fd, &this->_master_set);

	// print new connection and send welcome message to the channel

	return (0);
}

*/

// read from client


/*
void IRC::_event_search(int max_fd)
{
	for (int fd = 0; fd <= max_fd; fd++)
	{
		if (FD_ISSET(fd, &this->_read_set))
		{
			if (fd == this->_socket_fd)
				this->_accept_client();
			else
				this->_read_from_client(fd);
		}
		if (FD_ISSET(fd, &this->_write_set))
			this->_write_to_client(fd);
	}
}
*/
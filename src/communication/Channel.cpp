/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 22:03:57 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {
	// Constructor implementation
}

Channel::Channel(std::string name, std::string password): _name(name), _password(password) {
	// Constructor implementation
}

Channel::~Channel() {
	// Destructor implementation
}

bool	Channel::join(int client_fd, std::string password) {
    // Method implementation
    // if channel is invite only, check if client is invited
    // if channel is password protected, check if client has password
    // if channel is full (user_limit), reject client
    if (this->key_password && password != this->_password)
    {
        // Send error message ERR_BADCHANNELKEY (475) to client_fd
        return (false);
    }
    if (this->invite_only)
    {
        // Check that the client_fd is in the _invited list
    }
}

void    Channel::change_topic(std::string topic) {
    // Method implementation
    // if topic_locked, only operators can change the topic
    // if not, change the topic
}


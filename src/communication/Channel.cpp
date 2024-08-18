/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/18 21:16:54 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() {
	// Constructor implementation
}

Channel::Channel(std::string name, std::string password): invite_only(false), topic_locked(false), key_password(true), user_limit(0),
    _name(name), _password(password)
{
    // Constructor implementation
}

Channel::Channel(std::string name): invite_only(false), topic_locked(false), key_password(false), user_limit(0), _name(name), _password("")
{
    // Constructor implementation
}

Channel::~Channel() {
	// Destructor implementation
}

bool Channel::join(Client *client) {
    // if channel is password protected, check if client has password
    if (this->key_password)
    {
        std::string message = ":server.hostname 475 " + this->_name + " :Cannot join channel (+k)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list
        if (this->_invited.find(client->nickname) == this->_invited.end())
        {
            std::string message = ":server.hostname 473 " + this->_name + " :Cannot join channel (+i)";
            send(client->fd, message.c_str(), message.length(), 0);
            return (false);
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
        std::string message = ":server.hostname 471 " + this->_name + " :Cannot join channel (+l)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
    return (true);
}

bool Channel::join(Client *client, std::string password) {
    // if channel is password protected, check if client has password
    if (this->key_password && password != this->_password)
    {
        std::string message = ":server.hostname 475 " + this->_name + " :Cannot join channel (+k)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list
        if (this->_invited.find(client->nickname) == this->_invited.end())
        {
            std::string message = ":server.hostname 473 " + this->_name + " :Cannot join channel (+i)";
            send(client->fd, message.c_str(), message.length(), 0);
            return (false);
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
        std::string message = ":server.hostname 471 " + this->_name + " :Cannot join channel (+l)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
    return (true);
}

void    Channel::change_topic(Client *client, std::string topic) {
    // if topic_locked, only operators can change the topic
    // if not, change the topic
    if (this->topic_locked)
    {
        if (this->_operators.find(client->nickname) == this->_operators.end())
        {
            std::string message = ":server.hostname 482 " + this->_name + " :You're not channel operator";
            send(client->fd, message.c_str(), message.length(), 0);
            return ;
        }
    }
    this->topic = topic;
}


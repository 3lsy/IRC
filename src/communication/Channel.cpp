/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/24 13:49:38 by echavez-         ###   ########.fr       */
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

Channel::Channel(const Channel &other) {
	(void)other;
}

Channel& Channel::operator=(const Channel &other) {
	(void)other;
	return (*this);
}

Channel::~Channel() {
	// Destructor implementation
}

/**
 * @brief Sends a message to all members of the channel
 * 
 * @param message The message to send
 * 
 * @note Client format messages:
 * - :<nickname>!<username>@<hostname> JOIN :<channel>
 * - :<servername> 403 <nickname> <channel> :No such channel
 * - :<servername> 471 <nickname> <channel> :Cannot join channel (+l)
 * - :<servername> 473 <nickname> <channel> :Cannot join channel (+i)
 * - :<servername> 475 <nickname> <channel> :Cannot join channel (+k)
 */
bool Channel::join(Client *client) {
    // if channel is password protected, check if client has password
    if (this->key_password)
    {
        std::string message = ":" + std::string(SERVERNAME) + " 475 " + this->_name + " :Cannot join channel (+k)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list
        if (this->_invited.find(client->nickname) == this->_invited.end())
        {
            std::string message = ":" + std::string(SERVERNAME) + " 473 " + this->_name + " :Cannot join channel (+i)";
            send(client->fd, message.c_str(), message.length(), 0);
            return (false);
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
        std::string message = ":" + std::string(SERVERNAME) + " 471 " + this->_name + " :Cannot join channel (+l)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
    std::string message = ":" + client->nickname + "!" + client->username + "@" + client->hostname + " JOIN :" + this->_name + "\r\n";
    send(client->fd, message.c_str(), message.length(), 0);
    std::cout << BLUE << "SERVER: " << client->nickname << " joined " << this->_name << RESET << std::endl;
    return (true);
}

/**
 * @brief Sends a message to all members of the channel
 * 
 * @param message The message to send
 * 
 * @note Client format messages:
 * - :<nickname>!<username>@<hostname> JOIN :<channel>
 * - :<servername> 403 <nickname> <channel> :No such channel
 * - :<servername> 471 <nickname> <channel> :Cannot join channel (+l)
 * - :<servername> 473 <nickname> <channel> :Cannot join channel (+i)
 * - :<servername> 475 <nickname> <channel> :Cannot join channel (+k)
 */
bool Channel::join(Client *client, std::string password) {
    // if channel is password protected, check if client has password
    if (this->key_password && password != this->_password)
    {
        std::string message = ":" + std::string(SERVERNAME) + " 475 " + this->_name + " :Cannot join channel (+k)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list
        if (this->_invited.find(client->nickname) == this->_invited.end())
        {
            std::string message = ":" + std::string(SERVERNAME) + " 473 " + this->_name + " :Cannot join channel (+i)";
            send(client->fd, message.c_str(), message.length(), 0);
            return (false);
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
        std::string message = ":" + std::string(SERVERNAME) + " 471 " + this->_name + " :Cannot join channel (+l)";
        send(client->fd, message.c_str(), message.length(), 0);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
    std::string message = ":" + client->nickname + "!" + client->username + "@" + client->hostname + " JOIN :" + this->_name + "\r\n";
    send(client->fd, message.c_str(), message.length(), 0);
    std::cout << BLUE << "SERVER: " << client->nickname << " joined " << this->_name << RESET << std::endl;
    return (true);
}

/**
 * @brief Changes the topic of the channel
 * 
 * @param client The client changing the topic
 * @param topic The new topic
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> TOPIC <channel> :<topic>
 * @note Server format message: :<servername> 482 <channel> :You're not channel operator
 */
void    Channel::change_topic(Client *client, std::string topic) {
    if (this->topic_locked)
    {
        if (this->_operators.find(client->nickname) == this->_operators.end())
        {
            std::string message = ":" + std::string(SERVERNAME) + " 482 " + this->_name + " :You're not channel operator";
            send(client->fd, message.c_str(), message.length(), 0);
            return ;
        }
    }
    this->topic = topic;
}

// setters and getters
std::map<std::string, Client *> Channel::get_members(void) {
	return (this->_members);
}

std::map<std::string, Client *> Channel::get_operators(void) {
	return (this->_operators);
}

std::map<std::string, Client *> Channel::get_invited(void) {
	return (this->_invited);
}

std::string Channel::get_name(void) {
	return (this->_name);
}

bool Channel::get_invite_only(void) {
	return (this->invite_only);
}

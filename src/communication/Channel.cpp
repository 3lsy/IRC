/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/24 14:04:05 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() {
	// Constructor implementation
}

Channel::Channel(std::string name, std::string password, Client *client): invite_only(false), topic_locked(true), key_password(true), user_limit(0),
    _name(name), _password(password)
{
    this->_add_operator(client);
}

Channel::Channel(std::string name, Client *client): invite_only(false), topic_locked(true), key_password(false), user_limit(0), _name(name), _password("")
{
    this->_add_operator(client);
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
 * @brief Add operator to the channel
 * 
 * @param client The client to add as operator
 */
void    Channel::_add_operator(Client *client)
{
    this->_operators[client->nickname] = client;
    std::cout << BLUE << "SERVER: " << client->nickname << " is now an operator of " << this->_name << RESET << std::endl;
}

/**
 * @brief Sends a message to all members of the channel
 * 
 * @param message The message to send
 */
void    Channel::_broadcast(std::string message)
{
    for (std::map<std::string, Client *>::iterator it = this->_members.begin(); it != this->_members.end(); it++)
    {
        if (send(it->second->fd, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending message to client " << it->second->nickname << RESET << std::endl;
        }
    }
    std::cout << BLUE << "SERVER: Broadcasting message to all members of " << this->_name << RESET << std::endl;
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
    if (this->_members.find(client->nickname) == this->_members.end())
    {
        std::cout << RED << "SERVER: " << client->nickname << " is not a member of " << this->_name << RESET << std::endl;
        std::string message = ":" + std::string(SERVERNAME) + " 442 " + this->_name + " :You're not on that channel";
        if (send(client->fd, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending channel not found error message to client" << RESET << std::endl;
        }
        if (this->_operators.find(client->nickname) != this->_operators.end())
            this->_operators.erase(client->nickname);
        return ;
    }
    if (this->topic_locked)
    {
        if (this->_operators.find(client->nickname) == this->_operators.end())
        {
            std::cout << RED << "SERVER: " << client->nickname << " is not a channel operator" << RESET << std::endl;
            std::string message = ":" + std::string(SERVERNAME) + " 482 " + this->_name + " :You're not channel operator";
            send(client->fd, message.c_str(), message.length(), 0);
            return ;
        }
    }
    this->topic = topic;
    std::string message = ":" + client->nickname + "!" + client->username + "@" + client->hostname + " TOPIC " + this->_name + " " + topic + "\r\n";
    this->_broadcast(message);
}

void    Channel::get_topic(Client *client)
{
    std::cout << BLUE << "SERVER: TOPIC for " << this->_name << ": " << this->topic << RESET << std::endl;
    std::string message = ":" + std::string(SERVERNAME) + " 332 " + client->nickname + " " + this->_name + " " + this->topic + "\r\n";
    if (send(client->fd, message.c_str(), message.length(), 0) < 0)
    {
        std::cerr << RED << "SERVER: Error sending topic message to client" << RESET << std::endl;
    }
}

/**
 * @brief Changes the mode of the channel
 * 
 * @param client The client changing the mode
 * @param mode The mode to change
 * 
 * @note Client format message: :<nickname
 * @note Server format message: :<servername> 482 <channel> :You're not channel operator
 */
void    Channel::change_mode(Client *client, std::string mode)
{
    // Check if the client is an operator
    if (this->_operators.find(client->nickname) == this->_operators.end())
    {
        std::cout << RED << "SERVER: " << client->nickname << " is not a channel operator" << RESET << std::endl;
        std::string message = ":" + std::string(SERVERNAME) + " 482 " + this->_name + " :You're not channel operator\r\n";
        if (send(client->fd, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending channel not found error message to client" << RESET << std::endl;
        }
        return ;
    }
    if (mode[0] == '+')
    {
        if (mode[1] == 'i')
            this->invite_only = true;
        else if (mode[1] == 't')
            this->topic_locked = true;
    }
    else if (mode[0] == '-')
    {
        if (mode[1] == 'i')
            this->invite_only = false;
        else if (mode[1] == 't')
            this->topic_locked = false;
    }
}

/**
 * @brief Changes the mode of the channel
 * 
 * @param client The client changing the mode
 * @param mode The mode to change
 * @param arg The argument for the mode
 * 
 * @note Client format message: :<nickname
 * @note Server format message: :<servername> 482 <channel> :You're not channel operator
 */
void    Channel::change_mode(Client *client, std::string mode, std::string arg)
{
    if (this->_operators.find(client->nickname) == this->_operators.end())
    {
        std::cout << RED << "SERVER: " << client->nickname << " is not a channel operator" << RESET << std::endl;
        std::string message = ":" + std::string(SERVERNAME) + " 482 " + this->_name + " :You're not channel operator";
        if (send(client->fd, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending channel not found error message to client" << RESET << std::endl;
        }
        return ;
    }
    if (mode[0] == '+')
    {
        if (mode[1] == 'k')
        {
            this->key_password = true;
            this->_password = arg;
        }
        else if (mode[1] == 'l')
        {
            int limit = std::atoi(arg.c_str());
            if (arg.empty() || (limit == 0 && arg != "0"))
            {
                std::cerr << RED << "SERVER: Error: Invalid number format" << RESET << std::endl;
                std::string message = ":" + std::string(SERVERNAME) + " 461 " + _name + " :Not enough parameters";
                if (send(client->fd, message.c_str(), message.length(), 0) < 0)
                    std::cerr << RED << "SERVER: Error sending channel not found error message to client" << RESET << std::endl;
            }
            else
                user_limit = limit;
        }
    }
    else if (mode[0] == '-')
    {
        if (mode[1] == 'k')
        {
            this->key_password = false;
            this->_password = "";
        }
        else if (mode[1] == 'l')
        {
            this->user_limit = 0;
        }
    }
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

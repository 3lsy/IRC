/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/11/10 22:54:18 by echavez-         ###   ########.fr       */
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
void    Channel::broadcast(std::string message)
{
    std::cout << BLUE << "SERVER: Broadcasting message to all members of " << this->_name << RESET << std::endl;
    for (std::map<std::string, Client *>::iterator it = this->_members.begin(); it != this->_members.end(); it++)
    {
        if (send(it->second->fd, message.c_str(), message.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending message to client " << it->second->nickname << RESET << std::endl;
        }
        std::cout << YELLOW << "SERVER{" << it->second->fd;
        if (!it->second->nickname.empty())
            std::cout << it->second->nickname;
        std::cout << "}: " << message << RESET;
    }
}

void   Channel::update_member(std::string old_nickname, std::string new_nickname) {
    // Update the member's nickname in the channel
    if (this->_members.find(old_nickname) != this->_members.end()) {
        Client *client = this->_members[old_nickname];
        this->_members.erase(old_nickname);
        this->_members[new_nickname] = client;
        std::cout << BLUE << "SERVER: " << old_nickname << " changed nickname to " << new_nickname << " in channel " << this->_name << RESET << std::endl;
    }
    // Update the operator's nickname in the channel
    if (this->_operators.find(old_nickname) != this->_operators.end()) {
        Client *client = this->_operators[old_nickname];
        this->_operators.erase(old_nickname);
        this->_operators[new_nickname] = client;
        std::cout << BLUE << "SERVER: " << old_nickname << " changed nickname to " << new_nickname << " as operator in channel " << this->_name << RESET << std::endl;
    }
    // Update the invited list
    if (this->_invited.find(old_nickname) != this->_invited.end()) {
        Client *client = this->_invited[old_nickname];
        this->_invited.erase(old_nickname);
        this->_invited[new_nickname] = client;
        std::cout << BLUE << "SERVER: " << old_nickname << " changed nickname to " << new_nickname << " in invited list of channel " << this->_name << RESET << std::endl;
    }
    // Broadcast the nickname change to all members
    std::string message = ":" + old_nickname + " NICK :" + new_nickname + "\r\n";
    this->broadcast(message);
    std::cout << BLUE << "SERVER: Broadcasted nickname change from " << old_nickname << " to " << new_nickname << " in channel " << this->_name << RESET << std::endl;
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
    // if already a member, return false
    if (this->_members.find(client->nickname) != this->_members.end())
    {
        _print_error("Already a member", ":" + std::string(SERVERNAME) + " 443 " + this->_name + " :Already a member\r\n", client->fd);
        return (false);
    }
    // if channel is password protected, check if client has password
    if (this->key_password)
    {
		_print_error("Cannot join channel (+k)", ":" + std::string(SERVERNAME) + " 475 " + this->_name + " :Cannot join channel (+k)\r\n", client->fd);
        return (false);
    }
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list or the member list
        if (this->_invited.find(client->nickname) == this->_invited.end() && this->_members.find(client->nickname) == this->_members.end())
        {
			_print_error("Cannot join channel (+i)", ":" + std::string(SERVERNAME) + " 473 " + this->_name + " :Cannot join channel (+i)\r\n", client->fd);
            return (false);
        }
        else
        {
            this->_invited.erase(client->nickname);
            std::cout << BLUE << "SERVER: " << client->nickname << " is now a member of " << this->_name << RESET << std::endl;
            std::cout << BLUE << "SERVER: " << client->nickname << " is no longer invited to " << this->_name << RESET << std::endl;
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
		_print_error("Cannot join channel (+l)", ":" + std::string(SERVERNAME) + " 471 " + this->_name + " :Cannot join channel (+l)\r\n", client->fd);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
	this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " JOIN :" + this->_name + "\r\n");
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
    // if already a member, return false
    if (this->_members.find(client->nickname) != this->_members.end())
    {
        _print_error("Already a member", ":" + std::string(SERVERNAME) + " 443 " + this->_name + " :Already a member\r\n", client->fd);
        return (false);
    }
    // if channel is password protected, check if client has password
    if (this->key_password && password != this->_password)
    {
		_print_error("Cannot join channel (+k)", ":" + std::string(SERVERNAME) + " 475 " + this->_name + " :Cannot join channel (+k)\r\n", client->fd);
        return (false);
    }
    std::cout << BLUE << "SERVER: invite_only: " << (this->invite_only ? "true" : "false") << RESET << std::endl;
    // if channel is invite only, check if client is invited
    if (this->invite_only)
    {
        // Check that the client nick is in the _invited list or the member list
        if (this->_invited.find(client->nickname) == this->_invited.end() && this->_members.find(client->nickname) == this->_members.end())
        {
			_print_error("Cannot join channel (+i)", ":" + std::string(SERVERNAME) + " 473 " + this->_name + " :Cannot join channel (+i)\r\n", client->fd);
            return (false);
        }
        else
        {
            this->_invited.erase(client->nickname);
            std::cout << BLUE << "SERVER: " << client->nickname << " is now a member of " << this->_name << RESET << std::endl;
            std::cout << BLUE << "SERVER: " << client->nickname << " is no longer invited to " << this->_name << RESET << std::endl;
        }
    }
    // if channel is full (user_limit), reject client
    if (this->user_limit > 0 && this->_members.size() >= static_cast<size_t>(this->user_limit))
    {
		_print_error("Cannot join channel (+l)", ":" + std::string(SERVERNAME) + " 471 " + this->_name + " :Cannot join channel (+l)\r\n", client->fd);
        return (false);
    }
    // Add client to the channel
    this->_members[client->nickname] = client;
	this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " JOIN :" + this->_name + "\r\n");
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
		_print_error(client->nickname + " is not a member of " + this->_name, ":" + std::string(SERVERNAME) + " 442 " + this->_name + " :You're not on that channel\r\n", client->fd);
        if (this->_operators.find(client->nickname) != this->_operators.end())
            this->_operators.erase(client->nickname);
        return ;
    }
    if (this->topic_locked)
    {
        if (this->_operators.find(client->nickname) == this->_operators.end())
        {
			_print_error(client->nickname + " is not a channel operator", ":" + std::string(SERVERNAME) + " 482 " + this->_name + " :You're not channel operator\r\n", client->fd);
            return ;
        }
    }
    this->topic = topic;
    std::string message = ":" + client->nickname + "!" + client->username + "@" + client->hostname + " TOPIC " + this->_name + " " + topic + "\r\n";
    this->broadcast(message);
}

void    Channel::get_topic(Client *client)
{
	if (this->topic.empty())
		_print_message("No topic for " + this->_name, ":" + std::string(SERVERNAME) + " 331 " + client->nickname + " " + this->_name + " :No topic is set\r\n", client->fd);
	else
		_print_message("Sending topic " + this->topic + " to " + client->nickname, ":" + std::string(SERVERNAME) + " 332 " + client->nickname + " " + this->_name + " " + this->topic + "\r\n", client->fd);
}

/**
 * @brief Changes the mode of the channel
 * 
 * @param mode The mode to change
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> MODE <channel> <mode>
 * @note Server format message: :<servername> 482 <channel> :You're not channel operator
 */
void    Channel::change_mode(Client *client, std::string mode)
{

    if (mode.length() != 2 || (mode[0] != '+' && mode[0] != '-'))
    {
        std::string mode_without_sign = (mode.length() > 1) ? mode.substr(1) : mode;
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 472 " + _name + " " + mode_without_sign + " :is unknown mode\r\n", client->fd);
        return;
    }
    if (mode[1] == 'o' || mode[1] == 'k')
    {
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 461 " + _name + " " + mode + " :Not enough parameters\r\n", client->fd);
        return;
    }
    else if (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'l')
    {
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 472 " + _name + " " + mode.substr(1) + " :is unknown mode\r\n", client->fd);
        return;
    }
    if (mode[0] == '+')
    {
        if (mode[1] == 'i')
		{
            this->invite_only = true;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " +i\r\n");
			std::cout << BLUE << "SERVER: Channel is now invite only" << RESET << std::endl;
		}
        else if (mode[1] == 't')
		{
            this->topic_locked = true;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " +t\r\n");
			std::cout << BLUE << "SERVER: Channel topic is now locked" << RESET << std::endl;
		}
        else if (mode[1] == 'l')
        {
            _print_error("Invalid number format", ":" + std::string(SERVERNAME) + " 461 " + _name  + " " + mode + " :Not enough parameters\r\n", client->fd);
            return;
        }
    }
    else if (mode[0] == '-')
    {
        if (mode[1] == 'i')
		{
            this->invite_only = false;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " -i\r\n");
			std::cout << BLUE << "SERVER: Channel is no longer invite only" << RESET << std::endl;
		}
        else if (mode[1] == 'l')
        {
            this->user_limit = 0;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " -l\r\n");
			std::cout << BLUE << "SERVER: Channel user limit is now 0" << RESET << std::endl;
        }
        else if (mode[1] == 't')
		{
            this->topic_locked = false;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " -t\r\n");
			std::cout << BLUE << "SERVER: Channel topic is no longer locked" << RESET << std::endl;
		}
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
    if (mode.length() != 2 || (mode[0] != '+' && mode[0] != '-'))
    {
        std::string mode_without_sign = (mode.length() > 1) ? mode.substr(1) : mode;
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 472 " + _name + " " + mode_without_sign + " :is unknown mode\r\n", client->fd);
        return;
    }
    if (mode[1] == 'i' || mode[1] == 't')
    {
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 461 " + _name + " " + mode + " :Accepts no parameters\r\n", client->fd);
        return;
    }
    else if (mode[1] != 'k' && mode[1] != 'l' && mode[1] != 'o')
    {
        _print_error("Invalid mode syntax", ":" + std::string(SERVERNAME) + " 472 " + _name + " " + mode.substr(1) + " " + arg + " :is unknown mode\r\n", client->fd);
        return;
    }
    if (mode[0] == '+')
    {
        if (mode[1] == 'k')
        {
            this->key_password = true;
            this->_password = arg;
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " +k " + "\r\n");
			std::cout << BLUE << "SERVER: Channel is now password protected" << RESET << std::endl;
        }
        else if (mode[1] == 'l')
        {
            // arg size must be between 1 and 4 characters (error 696)
            if (arg.empty() || arg.length() > 4)
            {
                _print_error("Invalid number format", ":" + std::string(SERVERNAME) + " 696 " + _name + " " + mode + " :Limit must be a positive integer below 10,000\r\n", client->fd);
                return;
            }
            int limit = std::atoi(arg.c_str());
            if (limit == 0 && arg != "0")
				_print_error("Invalid number format", ":" + std::string(SERVERNAME) + " 461 " + _name + " " + mode + " :Not enough parameters", client->fd);
            else if (limit <= 0 || limit > 9999)
            {
                _print_error("Invalid number format", ":" + std::string(SERVERNAME) + " 696 " + _name + " " + mode + " :Limit must be a positive integer below 10,000\r\n", client->fd);
                return;
            }
            else 
			{
                user_limit = limit;
				this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " +l " + arg + "\r\n");
				std::cout << BLUE << "SERVER: Channel user limit is now " << limit << RESET << std::endl;
			}
        }
		else if (mode[1] == 'o')
		{
			if (this->_members.find(arg) != this->_members.end())
			{
				this->_add_operator(this->_members[arg]);
				this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " +o " + arg + "\r\n");
				std::cout << BLUE << "SERVER: " << arg << " is now an operator of " << this->_name << RESET << std::endl;
			}
			else
				_print_error("No such client", ":" + std::string(SERVERNAME) + " 401 " + client->nickname + " " + arg + " :No such client\r\n", client->fd);
		}
    }
    else if (mode[0] == '-')
    {
        if (mode[1] == 'k')
        {
            this->key_password = false;
            this->_password = "";
			this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " -k\r\n");
			std::cout << BLUE << "SERVER: Channel is no longer password protected" << RESET << std::endl;
        }
        else if (mode[1] == 'l')
        {
            // -l doesn't require an argument
            _print_error("Invalid number format", ":" + std::string(SERVERNAME) + " 461 " + _name + " " + mode + " :Accepts no parameters\r\n", client->fd);
        }
		else if (mode[1] == 'o')
		{
			if (this->_operators.find(arg) != this->_operators.end())
			{
				this->_operators.erase(arg);
				this->broadcast(":" + client->nickname + "!" + client->username + "@" + client->hostname + " MODE " + this->_name + " -o " + arg + "\r\n");
				std::cout << BLUE << "SERVER: " << arg << " is no longer an operator of " << this->_name << RESET << std::endl;
			}
			else
				_print_error("No such client", ":" + std::string(SERVERNAME) + " 401 " + client->nickname + " " + arg + " :No such client\r\n", client->fd);
		}
    }
}

/**
 * @brief Prints all members of the channel to the given client
 * 
 * @param client The client to send the member list to
 */
void Channel::print_members(Client *client) {
    std::string member_list;
    for (std::map<std::string, Client *>::iterator it = this->_members.begin(); it != this->_members.end(); ++it) {
        if (!member_list.empty())
            member_list += " ";
        // Prefix with @ if operator
        if (this->_operators.find(it->first) != this->_operators.end())
            member_list += "@";
        member_list += it->first;
    }
    std::string message = ":" + std::string(SERVERNAME) + " 353 " + client->nickname + " = " + this->_name + " :" + member_list + "\r\n";
    _print_message("Sending member list to " + client->nickname, message, client->fd);
    // End of names list
    std::string end_message = ":" + std::string(SERVERNAME) + " 366 " + client->nickname + " " + this->_name + " :End of /NAMES list\r\n";
    _print_message("End of member list for " + client->nickname, end_message, client->fd);
}


// setters and getters
std::map<std::string, Client *> &Channel::get_members(void) {
	return (this->_members);
}

std::map<std::string, Client *> &Channel::get_operators(void) {
	return (this->_operators);
}

std::map<std::string, Client *> &Channel::get_invited(void) {
	return (this->_invited);
}

std::string Channel::get_name(void) {
	return (this->_name);
}

bool Channel::get_invite_only(void) {
	return (this->invite_only);
}

std::string Channel::get_mode(void) {
    std::string mode = "+";
    if (this->invite_only)
        mode += "i";
    if (this->topic_locked)
        mode += "t";
    if (this->key_password)
        mode += "k";
    if (this->user_limit > 0)
    {
        std::ostringstream oss;
        oss << "l " << this->user_limit;
        mode += oss.str();
    }
    return mode;
}

void Channel::remove_member(std::string nickname) {
    std::map<std::string, Client *>::iterator it = _members.find(nickname);
    if (it != _members.end()) {
        _members.erase(it);
		_operators.erase(nickname);
        std::cout << BLUE << "SERVER: Removed " << nickname << " from channel " << _name << RESET << std::endl;
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_interactions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 19:47:39 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/18 23:26:20 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Client.hpp"
#include "Channel.hpp"

/**
 * @brief Handles the action process for the client
 * 
 * @param command The command received from the client
 * 
 * @format: <CMD> <PARAMS> :<TRAILING>
 * 
 * @note handled commands: PRIVMSG, JOIN, TOPIC, KICK, MODE, INVITE, QUIT
 * @note Ignore unhandled commands (e.g. PING, PONG, CAP, etc.)
 */
void    IRC::_interaction(std::string command, int client_fd)
{
    std::vector<std::string> cmd = split_cmd(command);
    if (cmd.size() == 0)
        return;
    if (cmd[0] == "JOIN")
    {
        if (cmd.size() == 2)
            this->_cmd_join(cmd[1], "", client_fd);
        else if (cmd.size() == 3)
            this->_cmd_join(cmd[1], cmd[2], client_fd);
        else
            return ;
    }
	else if (cmd[0] == "PRIVMSG")
	{
		if (cmd.size() == 3)
			this->_cmd_privmsg(cmd[1], cmd[2], client_fd);
		else
			return ;
	}
}

/**
 * @brief Handles the join process for the client
 * 
 * JOIN <channel>{,<channel>} [<key>{,<key>}]
 * 
 * @param channels The name of the channels to join
 * @param passwords The passwords for the channels
 * @param client_fd The file descriptor of the client
 * 
 * @todo Delete the channel in the destructor
 */
void    IRC::_cmd_join(std::string channels, std::string passwords, int client_fd)
{
    std::vector<std::string> chans = split_by(channels, ',');
    std::vector<std::string> pass = split_by(passwords, ',');
	if (pass.size() > 0 && chans.size() != pass.size())
		return ;
    for (size_t i = 0; i < chans.size(); i++)
    {
        if (this->_channels.find(chans[i]) == this->_channels.end())
        {
            // Channel does not exist, create it
            if (pass.size() <= i)
            {
                Channel *new_channel = new Channel(chans[i]);
                this->_channels[chans[i]] = new_channel;
            }
            else
            {
                Channel *new_channel = new Channel(chans[i], pass[i]);
                this->_channels[chans[i]] = new_channel;
            }
        }
        std::cout << BLUE << "SERVER: JOIN " << chans[i] << RESET << std::endl;
        if (pass.size() <= i)
            this->_channels[chans[i]]->join(this->_clients[client_fd]);
        else
            this->_channels[chans[i]]->join(this->_clients[client_fd], pass[i]);
    }
}

/**
 * @brief Handles the message process for the client and sends it to the target
 * 
 * @param target The target of the message
 * @param message The message to send
 * @param client_fd The file descriptor of the client
 */
void    IRC::_cmd_privmsg(std::string target, std::string message, int client_fd)
{
	if (target[0] == '#' || target[0] == '&')
	{
		if (this->_channels.find(target) != this->_channels.end()) {
			this->_send_to_channel(client_fd, this->_channels[target], message);
		}
		else {
			std::cerr << RED << "SERVER: Error: No such channel" << RESET << std::endl;
			std::string errorMessage = ":server.hostname 403 " + this->_clients[client_fd]->nickname + target + " :No such channel\r\n";
			if (send(client_fd, errorMessage.c_str(), errorMessage.length(), 0) < 0)
			{
				std::cerr << RED << "SERVER: Error sending channel not found error message to client" << RESET << std::endl;
			}
		}
	}
	else
	{
		if (this->_clients.find(this->_nicknames[target]) != this->_clients.end()) {
			this->_send_to_client(client_fd, this->_nicknames[target], message);
		}
		else {
			std::cerr << RED << "SERVER: Error: No such client" << RESET << std::endl;
			std::string errorMessage = ":server.hostname 401 " + this->_clients[client_fd]->nickname + target + " :No such client\r\n";
			if (send(client_fd, errorMessage.c_str(), errorMessage.length(), 0) < 0)
			{
				std::cerr << RED << "SERVER: Error sending client not found error message to client" << RESET << std::endl;
			}
		}
	}
}

void	IRC::_send_to_channel(int client_fd, Channel *channel, std::string message) {
	// with the channel name, search the channel in hashmap. It gives me the channel object whicn contains the list of members of the channel
	std::map<std::string, Client *> members = channel->get_members();
	std::map<std::string, Client *>::iterator it;
	(void)client_fd;
	for (it = members.begin(); it != members.end(); it++)
	{
		if (FD_ISSET(it->second->fd, &this->_write_set))// && it->second->fd != client_fd) // to debug, this returns the message to the sender too
		{
			std::cout << BLUE << "SERVER: Sending message to " << it->second->nickname << RESET << std::endl;
			std::string message_cli = ":" + this->_clients[client_fd]->nickname + " PRIVMSG " + channel->get_name() + " " + message + "\r\n";
			if (send(it->second->fd, message_cli.c_str(), message_cli.length(), 0) < 0)
			{
				std::cerr << RED << "SERVER: Error sending message to client" << RESET << std::endl;
			}
		}
	}
}

void	IRC::_send_to_client(int client_fd, int target_fd, std::string message) {
	if (FD_ISSET(target_fd, &this->_write_set) && target_fd != client_fd)
	{
		if (send(target_fd, message.c_str(), message.length(), 0) < 0)
		{
			std::cerr << RED << "SERVER: Error sending message to client" << RESET << std::endl;
		}
	}
}
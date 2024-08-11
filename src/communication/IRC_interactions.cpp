/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_interactions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 19:47:39 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 22:38:11 by echavez-         ###   ########.fr       */
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
            return;
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
 */
void    IRC::_cmd_join(std::string channels, std::string passwords, int client_fd)
{
    std::vector<std::string> chans = split_by(channels, ',');
    std::vector<std::string> pass = split_by(passwords, ',');
    for (size_t i = 0; i < chans.size(); i++)
    {
        if (this->_channels.find(chans[i]) == this->_channels.end())
        {
            // Channel does not exist, create it
            Channel *new_channel = new Channel(chans[i], pass[i]);
            this->_channels[chans[i]] = new_channel;
        }
        this->_channels[chans[i]]->join(client_fd, pass[i]);
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
			this->_channels[target]->send_message(target, message);
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
			this->_clients[this->_nicknames[target]]->send_message(client_fd, message);
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

void	Channel::send_message(std::string _channel_name, std::string message) {
}

void	Client::send_message(int client_fd, std::string message) {
	// Method implementation
	// Send message to client_fd
}
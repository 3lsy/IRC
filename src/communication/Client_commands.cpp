/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:12:26 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 16:01:53 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "IRC.hpp"

/**
 * @brief Splits a string into a vector of strings separated by spaces
 */
std::vector<std::string> split_cmd(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

/**
 * @brief This function handles the commands received from the client
 * 
 * @param command The command received from the client
 * 
 * @format: <CMD> <PARAMS> :<TRAILING>
 * 
 * @note Ignore unhandled commands (e.g. PING, PONG, CAP, etc.)
 */
void	Client::command_handler(std::string command)
{
	std::vector<std::string> cmd = split_cmd(command);
	if (cmd.size() == 0)
		return ;
	if (cmd[0] == "PASS")
	{
		if (cmd.size() < 2)
			return ;
		this->_cmd_pass(cmd[1]);
	}
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() < 2)
			return ;
		this->_cmd_nick(cmd[1]);
	}
	else if (cmd[0] == "USER")
	{
		if (cmd.size() < 5)
			return ;
		this->_cmd_user(cmd[1], cmd[2], cmd[3], cmd[4]);
	}
	else if (cmd[0] == "QUIT")
	{
		this->_cmd_quit();
	}

    if (this->_password && this->_nickname != "" && this->_username != "" && this->_realname != "")
    {
        std::string welcomeMessage = ":server.hostname 001 " + this->_nickname + " :Welcome to the Internet Relay Network " + this->_nickname + "!" + this->_username + "@" + this->_hostname + "\r\n";
        if (send(this->fd, welcomeMessage.c_str(), welcomeMessage.length(), 0) < 0) {
            std::cerr << "Error sending welcome message to client" << std::endl;
        }
        this->logged_in = true;
    }
}

/**
 * @brief Handles the PASS command
 * 
 * @param password The password received from the client
 */
void	Client::_cmd_pass(std::string password)
{
	this->_password = false;
	if (password == IRC::getInstance()->getPassword())
	{
		std::cout << "Password accepted" << std::endl;
		this->_password = true;
	}
	else
	{
		std::cout << "Password incorrect" << std::endl;
		std::string errorMessage = ":server.hostname 464 * :Password incorrect\r\n";
        if (send(this->fd, errorMessage.c_str(), errorMessage.length(), 0) < 0) {
            std::cerr << "Error sending password error message to client" << std::endl;
        }
		this->_cmd_quit();
	}
}

/**
 * @brief Handles the NICK command
 * 
 * @param nickname The nickname received from the client
 */
void	Client::_cmd_nick(std::string nickname)
{
    this->_nickname = nickname;
}

/**
 * @brief Handles the USER command
 * 
 * @param username The username received from the client
 * @param hostname The hostname received from the client
 * @param servername The servername received from the client
 * @param realname The realname received from the client
 */
void	Client::_cmd_user(std::string username, std::string hostname, 
                        std::string servername, std::string realname)
{
    this->_username = username;
    this->_hostname = hostname;
    this->_servername = servername;
    this->_realname = realname;
}

/**
 * @brief Handles the QUIT command
 */
void	Client::_cmd_quit(void)
{
    IRC::getInstance()->remove_client(this->fd);
}

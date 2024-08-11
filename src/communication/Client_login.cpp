/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_login.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:27:01 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 20:15:34 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "IRC.hpp"

/**
 * @brief Splits a string into a vector of strings separated by spaces
 */
std::vector<std::string>    split_cmd(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

std::vector<std::string>    split_by(const std::string& str, char delim)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (std::getline(stream, word, delim)) {
        result.push_back(word);
    }

    return result;
}

/**
 * @brief Handles the login process for the client
 * 
 * @param command The command received from the client
 * 
 * @format: <CMD> <PARAMS> :<TRAILING>
 * 
 * @note Ignore unhandled commands (e.g. PING, PONG, CAP, etc.)
 */
bool Client::login(std::string command)
{
    bool    success;

    success = true;
	std::vector<std::string> cmd = split_cmd(command);
	if (cmd.size() == 0)
		return (true);
	if (cmd[0] == "PASS")
	{
		if (cmd.size() < 2)
			return (false);
		success = this->_cmd_pass(cmd[1]);
	}
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() < 2)
			return (false);
		this->_cmd_nick(cmd[1]);
	}
	else if (cmd[0] == "USER")
	{
		if (cmd.size() < 5)
			return (false);
		this->_cmd_user(cmd[1], cmd[2], cmd[3], cmd[4]);
	}
    else if (cmd[0] == "QUIT")
    {
        this->_cmd_quit();
    }

    if (this->_password && this->nickname != "" && this->_username != "" && this->_realname != "")
    {
        std::string welcomeMessage = ":server.hostname 001 " + this->nickname + " :Welcome to the Internet Relay Network " + this->nickname + "!" + this->_username + "@" + this->_hostname + "\r\n";
        std::cout << BLUE << "SERVER: Sending welcome message to client: " << this->nickname << RESET << std::endl;
        if (send(this->fd, welcomeMessage.c_str(), welcomeMessage.length(), 0) < 0) {
            std::cerr << RED << "SERVER: Error sending welcome message to client" << RESET << std::endl;
        }
        this->logged_in = true;
    }
    return (success);
}

/**
 * @brief Handles the PASS command
 * 
 * @param password The password received from the client
 */
bool	Client::_cmd_pass(std::string password)
{
	if (password == IRC::getInstance()->getPassword())
	{
		std::cout << BLUE << "SERVER: Password accepted" << RESET << std::endl;
		this->_password = true;
        return (true);
	}
	else
	{
		std::cerr << RED << "SERVER: Password incorrect" << RESET << std::endl;
		std::string errorMessage = ":server.hostname 464 * :Password incorrect\r\n";
        if (send(this->fd, errorMessage.c_str(), errorMessage.length(), 0) < 0) {
            std::cerr << RED << "SERVER: Error sending password error message to client" << RESET << std::endl;
        }
	    this->_password = false;
		return (false);
	}
}

/**
 * @brief Handles the NICK command
 * 
 * @param nickname The nickname received from the client
 */
void	Client::_cmd_nick(std::string nickname)
{
    this->nickname = nickname;
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

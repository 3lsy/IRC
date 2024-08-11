/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_communication.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:05:51 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 16:07:25 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/**
 * @brief Handles the communication commands received from the client
 * 
 * @param command The command received from the client
 */
void	Client::communication_handler(std::string command)
{
	//check if the client is logged in and only allow communication commands if they are
}

/**
 * @brief Sends a message to the target client or channel
 * 
 * @param target The name of the target client or channel
 * @param message The message to send
 */
void Client::_cmd_privmsg(std::string target, std::string message)
{
	// Check if the target is a channel. Do this by checking if the target starts with a '#'
	// If it is, broadcast the message to all members of the channel
	// If it is not, send the message to the target client
	// If the target does not exist, send an error message to the client
}
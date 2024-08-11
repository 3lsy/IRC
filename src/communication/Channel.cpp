/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 19:04:50 by echavez-         ###   ########.fr       */
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

void	Channel::join(int client_fd) {
	// Method implementation
    // if channel does not exist, create it
    // if channel is invite only, check if client is invited
    // if channel is password protected, check if client has password
    // if channel is full (user_limit), reject client
}

void    Channel::change_topic(std::string topic) {
    // Method implementation
    // if topic_locked, only operators can change the topic
    // if not, change the topic

void	Channel::send_message(int client_fd, std::string message) {
	// Method implementation
}

void	Channel::_broadcast(std::string message) {
	// Method implementation
}

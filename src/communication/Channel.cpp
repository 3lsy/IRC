/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:53 by echavez-          #+#    #+#             */
/*   Updated: 2024/07/31 18:13:20 by echavez-         ###   ########.fr       */
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
}

void	Channel::leave(int client_fd) {
	// Method implementation
}

void	Channel::send_message(int client_fd, std::string message) {
	// Method implementation
}

void	Channel::_broadcast(std::string message) {
	// Method implementation
}

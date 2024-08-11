/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:33:12 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 16:51:34 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "lib.h"


class Client;

class Channel {
	private:
		// Attributes
		std::string		        _name;
		std::string	        	_password;
		std::map<int, Client *>	_members;
		std::map<int, Client *>	_operators;
		// Methods
		void	_broadcast(std::string message);
	public:
		// Methods
		Channel();
		Channel(std::string name, std::string password);
		~Channel();
		void	join(int client_fd);
		void	leave(int client_fd);
		void	send_message(int client_fd, std::string message);
};

#endif
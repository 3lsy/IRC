/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:33:12 by echavez-          #+#    #+#             */
/*   Updated: 2024/07/31 18:05:08 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "lib.h"

class Channel {
	private:
		// Attributes
		std::string		_name;
		std::string		_password;
		/*
		** Maybe a Client class will be created in the future
		** to handle the client's information and position in the channel, to differentiate between members and operators
		*/
		std::vector<int>	_members;
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
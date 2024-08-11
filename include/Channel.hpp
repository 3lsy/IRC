/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:33:12 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 19:05:10 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "lib.h"

class Client;

/** @brief Class to handle the channel object
 *
 * @note When the user limit is set, the channel will not accept more users than the limit
 *       It can be set to less than the people already in the channel, but no new users will be able to join
 */

class Channel {
	private:
		bool					invite_only; // i
		bool					topic_locked; // t
		bool					key_password; // k
		int						user_limit; // l
		// Attributes
		std::string				_name;
		std::string				_password;
		std::map<int, Client *>	_members;
		std::map<int, Client *>	_operators; // o
		// Methods
		void	_broadcast(std::string message);
	public:
		// Attributes
		std::string				topic;
		// Methods
		Channel();
		Channel(std::string name, std::string password);
		~Channel();
		void					join(int client_fd);
		void					send_message(int client_fd, std::string message);
		void					change_topic(std::string topic);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:33:12 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/24 13:11:34 by echavez-         ###   ########.fr       */
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
		std::map<std::string, Client *>	_members;
		std::map<std::string, Client *>	_operators; // o
		std::map<std::string, Client *> _invited;
		// Methods
		void	                _broadcast(std::string message);
	public:
		// Attributes
		std::string				topic;
		// Methods
		Channel();
        Channel(std::string name);
		Channel(std::string name, std::string password);
		Channel& operator=(const Channel &other);
		Channel(const Channel &other);
		~Channel();
		bool					join(Client *client);
		bool					join(Client *client, std::string password);
		void					send_message(std::string _channel_name, std::string message);
		void					change_topic(Client *client, std::string topic);
		// setters and getters
		std::map<std::string, Client *> get_members(void);
		std::map<std::string, Client *> get_operators(void);
		std::map<std::string, Client *> get_invited(void);
		std::string						get_name(void);
		bool							get_invite_only(void);
};

#endif

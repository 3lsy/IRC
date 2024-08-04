/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:38:37 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/03 12:06:45 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "lib.h"

class Client {
	private:
		// Attributes
		struct sockaddr_in	_socket_addr;
		socklen_t			_socket_len;
		// privileged channels
		// member channels
		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		std::string			_password;
		std::string			_hostname;
		std::string			_servername;
		std::string			_mode;
		std::string			_ip;
		std::string			_gecos;
		// Methods
		void	_parse_message(std::string message);
		void	_send_message(std::string message);
	public:
		// Attributes
		int					fd;
		struct sockaddr_in	socket_addr;
		socklen_t			socket_len;
		// Methods
		Client(void);
		Client(int fd);
		~Client(void);
		void	join_channel(std::string channel);
		void	leave_channel(std::string channel);
		void	send_message(std::string message);
};

#endif
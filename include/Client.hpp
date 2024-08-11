/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:38:37 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 15:07:01 by echavez-         ###   ########.fr       */
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
		bool				_password;
		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		std::string			_hostname;
		std::string			_servername;
		std::string			_mode;
		std::string			_ip;
		// Command methods
		void	_cmd_pass(std::string password);
		void	_cmd_nick(std::string nickname);
		void	_cmd_user(std::string username, std::string hostname, 
						std::string servername, std::string realname);
		void	_cmd_quit(void);
	public:
		// Attributes
        bool                logged_in;
		int					fd;
		struct sockaddr_in	socket_addr;
		socklen_t			socket_len;
		// Methods
		Client(void);
		Client(int fd);
		~Client(void);
		void	command_handler(std::string command);
};

#endif
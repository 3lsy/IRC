/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:38:37 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/11 17:56:05 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "lib.h"

class Channel;

class Client {
	private:
		// Attributes
		struct sockaddr_in	_socket_addr;
		socklen_t			_socket_len;
		// privileged channels
		// member channels
		std::map<std::string, Channel*> _channels;
		bool				_password;
		std::string			_username;
		std::string			_realname;
		std::string			_hostname;
		std::string			_servername;
		std::string			_mode;
		std::string			_ip;
		// Command methods
		bool	            _cmd_pass(std::string password);
		void	            _cmd_nick(std::string nickname);
		void	            _cmd_user(std::string username, std::string hostname, 
					            	std::string servername, std::string realname);
		void	            _cmd_quit(void);
		// Communication methods
		void	_cmd_privmsg(std::string target, std::string message);
	public:
		// Attributes
		int					fd;
		std::string			nickname;
        bool                logged_in;
		struct sockaddr_in	socket_addr;
		socklen_t			socket_len;
		// Methods
		Client(void);
		Client(int fd);
		~Client(void);
		//command handler for PASS, NICK, USER, QUIT
		bool	            login(std::string command);
		//command handler for communication (PRIVMSG, JOIN, PART, etc)
		void	            communication_handler(std::string command);
};

#endif

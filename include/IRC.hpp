/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:16:01 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/03 13:00:10 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "lib.h"
# include "Client.hpp"

class IRC {
private:
	// Attributes
	int					_port;
	std::string			_password;
	int					_socket_fd;
	int					_client_fd;
	struct sockaddr_in	_serv_addr;
	socklen_t			_serv_len;
	int					_max_fd;
	int					_n_clients;
	std::vector<Client *>	_clients;
	// fd_set for select
	fd_set				_read_set;
	fd_set				_write_set;
	fd_set				_master_set;
	// buffer
	char				_buffer[1024];
	int					_bytes_read;
	bool				_running;
	// Methods
	void	_create_socket(void);
	void	_bind_socket(void);
	void	_init_fd_sets(void);
	// Server methods
	void	_new_connection(void);
	void	_event_search(void);
	void	_read_from_client(int fd);
	void	_remove_client(int fd);

	// Static pointer to hold the singleton instance
    static IRC* instance;
	// Private constructor to prevent instantiation
	IRC(int port, std::string password);
	~IRC(void);

public:
	// Methods
	// Public static method to access the singleton instance
    static IRC*	getInstance(int port = 0, std::string password = "");

	// Server control methods
	void		stop_server(void);
	void		serve();
	// Static methods
	static bool	valid_port(int port);
	static void signal_handler(int signum);
};

#endif
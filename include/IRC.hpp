/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:16:01 by echavez-          #+#    #+#             */
/*   Updated: 2024/08/01 17:21:12 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "lib.h"

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
	// fd_set for select
	fd_set				_read_set;
	fd_set				_write_set;
	fd_set				_master_set;
	// Methods
	void	_create_socket();
	void	_bind_socket();
	void	_init_sets();
	// Server methods
	void	_accept_connections(); //for test only
	int		_accept_client();
	void	_read_from_client(int fd);
	void	_write_to_client(int fd);
	void	_event_search(int max_fd);

public:
	// Methods
	static bool	valid_port(int port);
	void		serve();

	IRC();
	IRC(int port, std::string password);
	~IRC();
};

#endif
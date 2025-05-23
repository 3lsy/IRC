/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 13:14:18 by echavez-          #+#    #+#             */
/*   Updated: 2024/11/10 21:09:30 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_HPP
# define LIB_HPP

# include <iostream>         // For standard I/O functions
# include <sys/types.h>      // For data types used in system calls
# include <sys/socket.h>     // For socket, setsockopt, bind, connect, listen, accept, send, recv
# include <netinet/in.h>     // For sockaddr_in, htons, htonl, ntohs, ntohl
# include <arpa/inet.h>      // For inet_addr, inet_ntoa
# include <netdb.h>          // For getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
# include <unistd.h>         // For close, lseek, fstat
# include <fcntl.h>          // For fcntl
# include <csignal>          // For signal, sigaction
# include <sys/select.h>     // For select
# include <cstring>          // For memset, memcpy
# include <cstdio>           // For standard I/O functions
# include <cstdlib>          // For general utilities like exit()
# include <vector> 
# include <set>
# include <string>
# include <sstream>
# include <cerrno>
# include <map>
# include <algorithm>

// ANSI color codes
# define RESET   "\033[0m"
# define BLUE    "\033[34m"
# define GREEN   "\033[32m"
# define RED     "\033[31m"
# define YELLOW  "\033[33m"
# define GRAY    "\033[90m"

# define SERVERNAME "IRC.42.fr"

std::vector<std::string>	split_cmd(const std::string& str);
std::vector<std::string>	split_by(const std::string& str, char delim);
void						_print_error(const std::string &context, const std::string &errorMessage, int client_fd = -1);
void						_print_message(const std::string &context, const std::string &message, int client_fd = -1);

#endif
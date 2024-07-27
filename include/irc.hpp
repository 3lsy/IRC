/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 11:58:25 by echavez-          #+#    #+#             */
/*   Updated: 2024/07/27 12:44:53 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

#include <iostream>         // For standard I/O functions
#include <sys/types.h>      // For data types used in system calls
#include <sys/socket.h>     // For socket, setsockopt, bind, connect, listen, accept, send, recv
#include <netinet/in.h>     // For sockaddr_in, htons, htonl, ntohs, ntohl
#include <arpa/inet.h>      // For inet_addr, inet_ntoa
#include <netdb.h>          // For getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
#include <unistd.h>         // For close, lseek, fstat
#include <fcntl.h>          // For fcntl
#include <csignal>          // For signal, sigaction
#include <sys/select.h>     // For select
#include <cstring>          // For memset, memcpy
#include <cstdio>           // For standard I/O functions
#include <cstdlib>          // For general utilities like exit()

#endif
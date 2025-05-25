#ifndef IRC_HPP
# define IRC_HPP

# include "lib.h"
# include "Client.hpp"
# include "Channel.hpp"

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
	std::map<int, Client *>             _clients;
	std::map<std::string, Channel *>    _channels;
	std::map<std::string, int>          _nicknames;
	// fd_set for select
	fd_set				_read_set;
	fd_set				_write_set;
	fd_set				_master_set;
	// buffer
	char				_buffer[1024 + 1];
	int					_bytes_read;
	bool				_running;
	// Methods
	void				_create_socket(void);
	void				_bind_socket(void);
	void				_init_fd_sets(void);
	// Server methods
	void				_new_connection(void);
	void				_event_search(void);
	void				_read_from_client(int fd);
	void				_read_client_message(int fd);
    //command handler for communication (PRIVMSG, JOIN, PART, etc)
    void                _interaction(std::string command, int fd);
	int	                _channel_member_type(std::string channel, int client_fd);
    void                _cmd_join(std::string channels, std::string passwords, int client_fd);
	void				_cmd_privmsg(std::string target, std::string message, int client_fd);
	void				_cmd_invite(std::string nickname, std::string channel, int client_fd);
	void				_cmd_kick(std::string channel, std::string nickname, std::string comment, int client_fd);
    void                _cmd_topic(std::string channel, std::string topic, int client_fd);
    void                _cmd_topic(std::string channel, int client_fd);
	void				_cmd_mode(std::string target, int client_fd);
    void                _cmd_mode(std::string channel, std::string mode, int client_fd);
    void                _cmd_mode(std::string channel, std::string mode, std::string arg, int client_fd);
	void				_cmd_nick(std::string nickname, int client_fd);
	//find methods
	bool				_find_user_by_nickname(std::string nickname);
	//send messages methods
	void				_send_to_channel(int client_fd, Channel *channel, std::string message);
	void				_send_to_client(int client_fd, int target_fd, std::string message);
	void				_logout_client(int client_fd);

	// Static pointer to hold the singleton instance
	static IRC*			instance;
	// Private constructor to prevent instantiation
	IRC(int port, std::string password);
	~IRC(void);

public:
	// Methods
	// Public static method to access the singleton instance
	static IRC*			getInstance(int port = 0, std::string password = "");

	// Server control methods
	void				stop_server(void);
	void				serve();
	void				remove_client(int fd, bool remove_nickname = true);
	// Static methods
	static bool			valid_port(int port);
	static void			signal_handler(int signum);
	//getter
	int					getPort(void) const;
	std::string			getPassword(void) const;
};

#endif
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
		std::string			_servername;
		std::string			_mode;
		std::string			_ip;
		// Command methods
		bool	            _cmd_pass(std::string password);
		void	            _cmd_nick(std::string nickname, std::map<std::string, int> nicknames);
		void	            _cmd_user(std::string username, std::string hostname, 
					            	std::string servername, std::string realname);
		void	            _cmd_quit(void);
	public:
		// Attributes
		int					fd;
		std::string			input_buffer;
		std::string			nickname;
		std::string			username;
		std::string			realname;
		std::string			hostname;
        bool                logged_in;
		struct sockaddr_in	socket_addr;
		socklen_t			socket_len;
		// Methods
		Client(void);
		Client(int fd, struct sockaddr_in addr);
		~Client(void);
		//command handler for PASS, NICK, USER, QUIT
		bool	            login(std::string command, std::map<std::string, int> nicknames, std::map<std::string, Channel*> channels);
		void				send_message(int client_fd, std::string message);
		bool				is_valid_nick(std::string& nick);
};

extern Client               *null_client_ptr;

#endif

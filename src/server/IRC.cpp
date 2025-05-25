#include "IRC.hpp"

IRC* IRC::instance = NULL;
static Client null_client;
Client* null_client_ptr = &null_client;

bool	IRC::valid_port(int port) {
	if (port < 1024 || port > 65535)
	{
		std::cerr << RED << "SERVER: Error: Invalid port" << RESET << std::endl;
		std::cerr << "Port must be a number between 1024 and 65535" << std::endl;
		return (false);
	}
	return (true);
}

void	IRC::signal_handler(int signum __attribute__((unused))) {
	std::cout << BLUE << "SERVER: Stopping server" << RESET << std::endl;
    instance->stop_server();
}

IRC::IRC(int port, std::string password):
	_port(port), _password(password), _n_clients(0), _running(true)
{
	int	opt;

	this->_create_socket();
	this->_bind_socket();
    opt = 1;
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << RED << "SERVER: Error: Unable to set socket options" << RESET << std::endl;
		exit(1);
	}
	if (listen(this->_socket_fd, 100) < 0)
	{
		std::cerr << RED << "SERVER: Error: Unable to listen on socket" << RESET << std::endl;
		exit(1);
	}
}

IRC::~IRC(void) {
	std::cout << BLUE << "SERVER: IRC server shutting down" << RESET << std::endl;

	// Clean up clients
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		delete it->second;
	}
	_clients.clear();
	// Clean up channels
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second;
	}
	_channels.clear();
	if (this->_socket_fd >= 0) {
		close(this->_socket_fd);
		std::cout << BLUE << "SERVER: Server socket closed" << RESET << std::endl;
	}
}

IRC* IRC::getInstance(int port, std::string password) {
    if (instance == NULL) {
        instance = new IRC(port, password);
    }
    return instance;
}

std::string IRC::getPassword(void) const {
	return this->_password;
}

int IRC::getPort(void) const {
	return this->_port;
}

void	IRC::_logout_client(int client_fd)
{
	// nullify client values using null_client_ptr in the member and operator maps in each channel, but leave the keys intact
	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		Channel* channel = it->second;
		if (channel) {
			std::map<std::string, Client*>& members = channel->get_members();
			std::map<std::string, Client*>& operators = channel->get_operators();
			if (members.find(this->_clients[client_fd]->nickname) != members.end()) {
				members[this->_clients[client_fd]->nickname] = null_client_ptr;
			}
			if (operators.find(this->_clients[client_fd]->nickname) != operators.end()) {
				operators[this->_clients[client_fd]->nickname] = null_client_ptr;
			}
		}
	}
}
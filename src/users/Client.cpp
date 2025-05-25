#include "Client.hpp"

Client::Client(void) : _password(false), _servername(""), _mode(""), _ip(""), input_buffer(""), nickname(""),
                                username(""), realname(""), hostname(""), logged_in(false)
{
}

Client::Client(int fd, struct sockaddr_in addr) : _password(false), _servername(""), _mode(""), _ip(""), fd(fd), input_buffer(""), nickname(""),
                                username(""), realname(""), hostname(""), logged_in(false), socket_addr(addr)
{
	this->socket_len = sizeof(this->socket_addr);
}

Client::~Client(void) {
	if (this->fd >= 0) {
        close(this->fd);
        std::cout << BLUE << "SERVER: Client socket closed" << RESET << std::endl;
    }
}

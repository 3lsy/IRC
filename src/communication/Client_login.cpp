#include "Client.hpp"
#include "IRC.hpp"

/**
 * @brief Splits a string into a vector of strings separated by spaces
 */
std::vector<std::string>    split_cmd(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

std::vector<std::string>    split_by(const std::string& str, char delim)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (std::getline(stream, word, delim)) {
        result.push_back(word);
    }

    return result;
}

/**
 * @brief Handles the login process for the client
 * 
 * @param command The command received from the client
 * 
 * @format: <CMD> <PARAMS> :<TRAILING>
 * 
 * @note Ignore unhandled commands (e.g. PING, PONG, CAP, etc.)
 * @note Client welcome message: :<servername> 001 <nickname> :Welcome to the Internet Relay Network <nickname>!<username>@<hostname>
 */
bool Client::login(std::string command, std::map<std::string, int> nicknames, std::map<std::string, Channel*> channels)
{
    bool    success;

    success = true;
	std::vector<std::string> cmd = split_cmd(command);
	if (cmd.size() == 0)
		return (true);
	if (cmd[0] == "PASS")
	{
		if (cmd.size() < 2)
        {
            std::cout << BLUE << "SERVER: Invalid number of PASS parameters" << RESET << std::endl;
			return (false);
        }
		success = this->_cmd_pass(cmd[1]);
	}
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() < 2)
        {
            std::cout << BLUE << "SERVER: Invalid number of NICK parameters" << RESET << std::endl;
			return (false);
        }
		this->_cmd_nick(cmd[1], nicknames);
	}
	else if (cmd[0] == "USER")
	{
		if (cmd.size() < 5)
        {
            std::cout << BLUE << "SERVER: Invalid number of USER parameters" << RESET << std::endl;
			return (false);
        }
		this->_cmd_user(cmd[1], cmd[2], cmd[3], cmd[4]);
	}
    else if (cmd[0] == "QUIT")
    {
        this->_cmd_quit();
    }
    else if (cmd[0] == "CAP")
    {
        return (true);
    }
    else
    {
        _print_error("Unknown command", ":" + std::string(SERVERNAME) + " 421 " + cmd[0] + " :Unknown command\r\n", this->fd);
    }

    if (this->_password && this->nickname != "" && this->username != "" && this->realname != "")
    {

        std::string welcomeMessage = ":" + std::string(SERVERNAME) + " 001 " + this->nickname + " :Welcome to the Internet Relay Network " + this->nickname + "!" + this->username + "@" + this->hostname + "\r\n";
        std::cout << BLUE << "SERVER: Sending welcome message to client: " << this->nickname << RESET << std::endl;
        std::cout << YELLOW << "SERVER{";
        if (!this->nickname.empty())
            std::cout << this->nickname;
        else
            std::cout << this->fd;
        std::cout << "} " << welcomeMessage << RESET;
        if (send(this->fd, welcomeMessage.c_str(), welcomeMessage.length(), 0) < 0) {
            std::cerr << RED << "SERVER: Error sending welcome message to client" << RESET << std::endl;
        }
        this->logged_in = true;
        // Update every channel's member and operator list with this client
        for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
            if (it->second->get_members().find(this->nickname) != it->second->get_members().end()) {
                it->second->get_members()[this->nickname] = this;
                this->_channels[it->second->get_name()] = it->second;
                _print_message("Sending JOIN message to client", ":" + this->nickname + "!" + this->username + "@" + this->hostname + " JOIN :" + it->second->get_name() + "\r\n", this->fd);
            }
            if (it->second->get_operators().find(this->nickname) != it->second->get_operators().end()) {
                it->second->get_operators()[this->nickname] = this;
                std::string modeMessage = ":" + this->nickname + "!" + this->username + "@" + this->hostname + " MODE " + it->second->get_name() + " +o " + this->nickname + "\r\n";
                _print_message("Sending MODE +o message to client", modeMessage, this->fd);
            }
        }
    }
    return (success);
}

/**
 * @brief Handles the PASS command
 * 
 * @param password The password received from the client
 */
bool	Client::_cmd_pass(std::string password)
{
	if (password == IRC::getInstance()->getPassword())
	{
		std::cout << BLUE << "SERVER: Password accepted" << RESET << std::endl;
		this->_password = true;
        return (true);
	}
	else
	{
		std::cerr << RED << "SERVER: Password incorrect" << RESET << std::endl;
		std::string errorMessage = ":" + std::string(SERVERNAME) + " 464 * :Password incorrect\r\n";
        std::cout << YELLOW << "SERVER{" << this->fd << "}: " << errorMessage << RESET;
        if (send(this->fd, errorMessage.c_str(), errorMessage.length(), 0) < 0) {
            std::cerr << RED << "SERVER: Error sending password error message to client" << RESET << std::endl;
        }
	    this->_password = false;
		return (false);
	}
}

/**
 * @brief Handles the NICK command
 * 
 * @param nickname The nickname received from the client
 */
void	Client::_cmd_nick(std::string nickname, std::map<std::string, int> nicknames)
{
    if (!is_valid_nick(nickname))
    {
        _print_error("Invalid nickname", ":" + std::string(SERVERNAME) + " 432 " + this->nickname + " " + nickname + " :Erroneous nickname\r\n", this->fd);
        return ;
    }
    if (nicknames.find(nickname) != nicknames.end())
	{
		_print_error("Nickname already in use", ":" + std::string(SERVERNAME) + " 433 * " + nickname + " :Nickname is already in use\r\n", this->fd);
		return ;
	}
    this->nickname = nickname;
}

/**
 * @brief Handles the USER command
 * 
 * @param username The username received from the client
 * @param hostname The hostname received from the client
 * @param servername The servername received from the client
 * @param realname The realname received from the client
 */
void	Client::_cmd_user(std::string username, std::string hostname, 
                        std::string servername, std::string realname)
{
    this->username = username;
    this->hostname = hostname;
    this->_servername = servername;
    this->realname = realname;
}

/**
 * @brief Handles the QUIT command
 */
void	Client::_cmd_quit(void)
{
    IRC::getInstance()->remove_client(this->fd);
}

bool    Client::is_valid_nick(std::string& nick)
{ 
    if (nick.empty() || nick.length() > 9)
        return false;

    for (size_t i = 0; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '[' && c != ']' && c != '\\' &&
            c != '`' && c != '^' && c != '{' && c != '}') {
            return false;
        }
    }
    return true;
}
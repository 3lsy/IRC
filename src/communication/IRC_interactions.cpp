/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC_interactions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 19:47:39 by echavez-          #+#    #+#             */
/*   Updated: 2024/11/10 23:12:05 by echavez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Client.hpp"
#include "Channel.hpp"

/**
 * @brief Prints an error message to the client
 * 
 * @param context The context of the error
 * @param message The message to print
 * @param client_fd The file descriptor of the client
 */
void _print_error(const std::string &context, const std::string &errorMessage, int client_fd)
{
	std::cerr << RED << "SERVER: Error: " << context << RESET << std::endl;
	std::cerr << YELLOW << "SERVER{" << client_fd << "}: " << errorMessage << RESET;
    if (client_fd != -1)
    {
        if (send(client_fd, errorMessage.c_str(), errorMessage.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending error message to client" << RESET << std::endl;
        }
    }
}

void _print_message(const std::string &context, const std::string &message, int client_fd)
{
	std::cout << BLUE << "SERVER: " << context << RESET << std::endl;
	std::cout << YELLOW << "SERVER{" << client_fd << "}: " << message << RESET;
	if (client_fd != -1)
	{
		if (send(client_fd, message.c_str(), message.length(), 0) < 0)
		{
			std::cerr << RED << "SERVER: Error sending message to client" << RESET << std::endl;
		}
	}
}

/**
 * @brief Checks the type of member in a channel
 * 
 * @param channel The channel to check
 * @param client_fd The file descriptor of the client
 * 
 * @return 0 if not a member, 1 if member, 2 if operator, -1 if channel not found
 */
int IRC::_channel_member_type(std::string channel, int client_fd)
{
	if (this->_channels.find(channel) == this->_channels.end())
		return -1;
	std::map<std::string, Client *> members = this->_channels[channel]->get_members();
	std::map<std::string, Client *> operators = this->_channels[channel]->get_operators();

	if (operators.find(this->_clients[client_fd]->nickname) != operators.end())
		return 2;
	else if (members.find(this->_clients[client_fd]->nickname) != members.end())
		return 1;
	return 0;
}
/**
 * @brief Handles the action process for the client
 * 
 * @param command The command received from the client
 * 
 * @format: <CMD> <PARAMS> :<TRAILING>
 * 
 * @note handled commands: PRIVMSG, JOIN, TOPIC, KICK, MODE, INVITE, QUIT
 * @note Ignore unhandled commands (e.g. PING, PONG, CAP, etc.)
 * 
 * JOIN: <channel[,channel,...]> [<key>[,<key>,...]]
 * TOPIC: <channel> [<topic>]
 * KICK: <channel> <nickname> [<comment>]
 * MODE: <channel> [<mode> [<mode params>]]
 * 
 * PRIVMSG: <nickname> <text to be sent>
 * INVITE: <nickname> <channel>
 * NICK: <nickname>
 * 
 */
void    IRC::_interaction(std::string command, int client_fd)
{
    std::vector<std::string> cmd = split_cmd(command);
    if (cmd.size() == 0)
	{
        return ;
	}
	
	if ((cmd.size() < 2) && (cmd[0] == "MODE" || cmd[0] == "INVITE" || cmd[0] == "KICK" || cmd[0] == "TOPIC" || cmd[0] == "JOIN"))
	{
		_print_error("Incorrect command", ":" + std::string(SERVERNAME) + " 461 " + cmd[0] + " :Not enough parameters\r\n", client_fd);
		return ;
	}

    if (cmd[0] == "JOIN")
    {
        if (cmd.size() == 2)
            this->_cmd_join(cmd[1], "", client_fd);
        else if (cmd.size() == 3)
            this->_cmd_join(cmd[1], cmd[2], client_fd);
		else if (cmd.size() > 3)
			_print_error("Too many parameters", ":" + std::string(SERVERNAME) + " 461 " + cmd[1] + " " + cmd[0] + " :Too many parameters\r\n", client_fd);
		return ;
    }
	else if (cmd[0] == "PRIVMSG")
	{
		if (cmd.size() >= 3)
        {
            std::string message = command.substr(command.find(cmd[2]));
            this->_cmd_privmsg(cmd[1], message, client_fd);
        }
		else if (cmd.size() == 2)
		{
			_print_error("No message given", ":" + std::string(SERVERNAME) + " 411 " + cmd[1] + " :No text to send\r\n", client_fd);
			return ;
		}
		else {
			return ;
		}
	}
	else if (cmd[0] == "INVITE")
	{
		if (cmd.size() == 3)
		{
			int member_type = this->_channel_member_type(cmd[2], client_fd);
			if (member_type != 2)
			{
				if (member_type == 0)
					_print_error("Not on channel", ":" + std::string(SERVERNAME) + " 442 " + cmd[1] + " :You're not on that channel\r\n", client_fd);
				else if (member_type == -1)
					_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + cmd[1] + " :No such channel\r\n", client_fd);
				else
					_print_error("Not channel operator", ":" + std::string(SERVERNAME) + " 482 " + cmd[1] + " :You're not channel operator\r\n", client_fd);
				return ;
			}
				this->_cmd_invite(cmd[1], cmd[2], client_fd);
		}
		else
			_print_error("Incorrect number of parameters", ":" + std::string(SERVERNAME) + " 461 " + cmd[0] + " :Incorrect number of parameters\r\n", client_fd);
		return ;	
	}
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() == 2)
		{
			this->_cmd_nick(cmd[1], client_fd);
		}
		else
			_print_error("Incorrect NICK command", ":" + std::string(SERVERNAME) + " 431 * :No nickname given\r\n", client_fd);
		return ;
	}
	else if (cmd[0] == "KICK")
	{
		if (cmd.size() < 3)
		{
			_print_error("Incorrect KICK command", ":" + std::string(SERVERNAME) + " 461 KICK :Not enough parameters\r\n", client_fd);
			return ;
		}
		if (cmd.size() >= 3)
		{
			int member_type = this->_channel_member_type(cmd[1], client_fd);
			if (member_type != 2)
			{
				if (member_type == 0)
					_print_error("Not on channel", ":" + std::string(SERVERNAME) + " 442 " + cmd[1] + " :You're not on that channel\r\n", client_fd);
				else if (member_type == -1)
					_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + cmd[1] + " :No such channel\r\n", client_fd);
				else
					_print_error("Not channel operator", ":" + std::string(SERVERNAME) + " 482 " + cmd[1] + " :You're not channel operator\r\n", client_fd);
				return ;
			}
		}
		if (cmd.size() == 3)
		{
			this->_cmd_kick(cmd[1], cmd[2], "", client_fd);
		}
		else if (cmd.size() >= 4)
		{
			std::string comment = command.substr(command.find(cmd[3]));
			this->_cmd_kick(cmd[1], cmd[2], comment, client_fd);
		}
		return ;
	}
    else if (cmd[0] == "TOPIC")
    {
        if (cmd.size() >= 3)
        {
            std::string topic = command.substr(command.find(cmd[2]));
            this->_cmd_topic(cmd[1], topic, client_fd);
        }
        else if (cmd.size() == 2)
            this->_cmd_topic(cmd[1], client_fd);
		return ;
    }
    else if (cmd[0] == "MODE")
    {
		int member_type = this->_channel_member_type(cmd[1], client_fd);
		if (member_type != 2)
		{
			if (member_type == 0)
				_print_error("Not on channel", ":" + std::string(SERVERNAME) + " 442 " + cmd[1] + " :You're not on that channel\r\n", client_fd);
			else if (member_type == -1)
				_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + cmd[1] + " :No such channel\r\n", client_fd);
			else
				_print_error("Not channel operator", ":" + std::string(SERVERNAME) + " 482 " + cmd[1] + " :You're not channel operator\r\n", client_fd);
			return ;
		}
        if (cmd.size() == 3)
            this->_cmd_mode(cmd[1], cmd[2], client_fd);
        else if (cmd.size() == 4)
            this->_cmd_mode(cmd[1], cmd[2], cmd[3], client_fd);
		else
			_print_error("Incorrect number of parameters", ":" + std::string(SERVERNAME) + " 461 " + cmd[1] + " " + cmd[0] + " :Incorrect number of parameters\r\n", client_fd);
		return ;
    }
    else if (cmd[0] == "PING")
    {
        std::string pong = "PONG " + std::string(SERVERNAME) + "\r\n";
        if (send(client_fd, pong.c_str(), pong.length(), 0) < 0)
        {
            std::cerr << RED << "SERVER: Error sending PONG message to client " << this->_clients[client_fd]->nickname << RESET << std::endl;
        }
		std::cout << GRAY << "SERVER{";
		if (this->_clients[client_fd] && !this->_clients[client_fd]->nickname.empty())
			std::cout << this->_clients[client_fd]->nickname;
		else
			std::cout << client_fd;
		std::cout << "}: " << pong << RESET;
    }
	else if (cmd[0] == "QUIT")
	{
		return ;
	}
	else
	{
		_print_error("Unknown command", ":" + std::string(SERVERNAME) + " 421 " + cmd[0] + " :Unknown command\r\n", client_fd);
		return ;
	}
}

/**
 * @brief Handles the join process for the client
 * 
 * JOIN <channel>{,<channel>} [<key>{,<key>}]
 * 
 * @param channels The name of the channels to join
 * @param passwords The passwords for the channels
 * @param client_fd The file descriptor of the client
 * 
 */
void    IRC::_cmd_join(std::string channels, std::string passwords, int client_fd)
{
    std::vector<std::string> chans = split_by(channels, ',');
    std::vector<std::string> pass = split_by(passwords, ',');
	
	std::cout << BLUE << "SERVER: chans.size() = " << chans.size() << ", pass.size() = " << pass.size() << RESET << std::endl;
	if (pass.size() > 0 && chans.size() != pass.size())
	{
		_print_error("Invalid JOIN command", ":" + std::string(SERVERNAME) + " 461 JOIN :Not enough parameters\r\n", client_fd);
		return ;
	}
    // Check channel names contain # or & at the beginning
    for (size_t i = 0; i < chans.size(); i++)
    {
        if (chans[i][0] != '#' && chans[i][0] != '&')
        {
			_print_error("Invalid channel name", ":" + std::string(SERVERNAME) + " 403 " + chans[i] + " :No such channel\r\n", client_fd);
			return ;
        }
    }
    for (size_t i = 0; i < chans.size(); i++)
    {
		bool channel_created = false;
        if (this->_channels.find(chans[i]) == this->_channels.end())
        {
            // Channel does not exist, create it
            if (pass.size() <= i)
            {
                Channel *new_channel = new Channel(chans[i], this->_clients[client_fd]);
                this->_channels[chans[i]] = new_channel;
            }
            else
            {
                Channel *new_channel = new Channel(chans[i], pass[i], this->_clients[client_fd]);
                this->_channels[chans[i]] = new_channel;
            }
			channel_created = true;
        }
        std::cout << BLUE << "SERVER: JOIN " << chans[i] << RESET << std::endl;
		bool joined = false;
        if (pass.size() <= i)
            joined = this->_channels[chans[i]]->join(this->_clients[client_fd]);
        else
            joined = this->_channels[chans[i]]->join(this->_clients[client_fd], pass[i]);
		
		// After joining the channel
		// Make the client an operator if the channel was just created
		if (channel_created)
		{
			this->_channels[chans[i]]->broadcast(
				":" + this->_clients[client_fd]->nickname + '!' +
				this->_clients[client_fd]->username + '@' +
				this->_clients[client_fd]->hostname + " MODE " +
				chans[i] + " +o " + this->_clients[client_fd]->nickname + "\r\n");
		}
		// Send the channel topic to the client if successfuly joined
		if (joined && this->_channel_member_type(chans[i], client_fd) > 0)
		{
			this->_channels[chans[i]]->get_topic(this->_clients[client_fd]);
			this->_channels[chans[i]]->print_members(this->_clients[client_fd]);
		}
    }
}

/**
 * @brief Handles the message process for the client and sends it to the target
 * 
 * @param target The target of the message
 * @param message The message to send
 * @param client_fd The file descriptor of the client
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> PRIVMSG <target> :<message>
 * @note Server format message: :<servername> 403 <nickname> <target> :No such channel
 */
void    IRC::_cmd_privmsg(std::string target, std::string message, int client_fd)
{
	if (target[0] == '#' || target[0] == '&')
	{
		if (this->_channels.find(target) != this->_channels.end()) {
			if (this->_channel_member_type(target, client_fd) == 0)
			{
				_print_error("Not on channel", ":" + std::string(SERVERNAME) + " 442 " + this->_clients[client_fd]->nickname + " " + target + " :You're not on that channel\r\n", client_fd);
				return ;
			}
			this->_send_to_channel(client_fd, this->_channels[target], message);
		}
		else {
			_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + this->_clients[client_fd]->nickname + target + " :No such channel\r\n", client_fd);
		}
	}
	else
	{
		if (this->_find_user_by_nickname(target)) {
			this->_send_to_client(client_fd, this->_nicknames[target], message);
		}
		else {
			_print_error("No such client", ":" + std::string(SERVERNAME) + " 401 " + this->_clients[client_fd]->nickname + " " + target + " :No such client\r\n", client_fd);
		}
	}
}

/*
find the user by nickname, bool function
*/
bool IRC::_find_user_by_nickname(std::string nickname)
{
	if (this->_nicknames.find(nickname) != this->_nicknames.end())
		return true;
	return false;
}

/**
 * @brief Sends a message to all the members of a channel
 * 
 * @param client_fd The file descriptor of the client
 * @param channel The channel to send the message to
 * @param message The message to send
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> PRIVMSG <channel> :<message>
 */
void	IRC::_send_to_channel(int client_fd, Channel *channel, std::string message) {
	std::map<std::string, Client *> members = channel->get_members();
	std::map<std::string, Client *>::iterator it;
	(void)client_fd;
	for (it = members.begin(); it != members.end(); it++)
	{
		if (FD_ISSET(it->second->fd, &this->_write_set) && it->second->fd != client_fd)
		{
			_print_message("Sending message to " + it->second->nickname, ":" + this->_clients[client_fd]->nickname + '!' + this->_clients[client_fd]->username + '@' + this->_clients[client_fd]->hostname + " PRIVMSG " + channel->get_name() + " " + message + "\r\n", it->second->fd);
		}
	}
}

/**
 * @brief Sends a message to a client
 * 
 * @param client_fd The file descriptor of the client sending the message
 * @param target_fd The file descriptor of the target client
 * @param message The message to send
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> PRIVMSG <target> :<message>
 */
void	IRC::_send_to_client(int client_fd, int target_fd, std::string message) {
	std::string formatted_msg = ":" + this->_clients[client_fd]->nickname + '!' +
		this->_clients[client_fd]->username + '@' + this->_clients[client_fd]->hostname +
		" PRIVMSG " + this->_clients[target_fd]->nickname + " " + message + "\r\n";

	if (FD_ISSET(target_fd, &this->_write_set)) {
		_print_message("Sending message to " + this->_clients[target_fd]->nickname, formatted_msg, target_fd);
	}
}

/**
 * @brief Handles the invite process for the client
 * 
 * @param nickname The nickname of the client to invite
 * @param channel The channel to invite the client to
 * @param client_fd The file descriptor of the client
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> INVITE <nickname> <channel>
 */
void    IRC::_cmd_invite(std::string nickname, std::string channel, int client_fd) {
	std::map<std::string, Client *> members = this->_channels[channel]->get_members();
	std::map<std::string, Client *> operators = this->_channels[channel]->get_operators();
	//check if nickname exists
	if (this->_nicknames.find(nickname) == this->_nicknames.end())
	{
		_print_error("No such nickname", ":" + std::string(SERVERNAME) + " 401 " + this->_clients[client_fd]->nickname + " " + nickname + " :No such nickname\r\n", client_fd);
		return ;
	}
	//check if nickname is already in channel
	else if (members.find(nickname) != members.end())
	{
		_print_error("Already in channel", ":" + std::string(SERVERNAME) + " 443 " + this->_clients[client_fd]->nickname + " " + nickname + " " + channel + " :is already on channel\r\n", client_fd);
		return ;
	}
	//send invite message
	else
	{
		Client* inviter = this->_clients[client_fd];
		Client* invited = this->_clients[this->_nicknames[nickname]];

		// Format INVITE message to send to the invited client
		std::string invite_msg = ":" + inviter->nickname + "!" + inviter->username + "@" + inviter->hostname + " INVITE " + nickname + " :" + channel + "\r\n";
		_print_message("Sending INVITE to " + nickname, invite_msg, invited->fd);

		// Format confirmation message to send back to the inviter
		std::string confirm_msg = ":" + std::string(SERVERNAME) + " 341 " + inviter->nickname + " " + nickname + " " + channel + "\r\n";
		_print_message("INVITE confirmation to inviter", confirm_msg, client_fd);

		// Add to invited list
		this->_channels[channel]->get_invited()[nickname] = invited;
	}
}

void	IRC::_cmd_nick(std::string nickname, int client_fd)
{
	Client* client = this->_clients[client_fd];

	if (!client->is_valid_nick(nickname))
	{
		_print_error("Invalid nickname", ":" + std::string(SERVERNAME) + " 432 " + this->_clients[client_fd]->nickname + " " + nickname + " :Erroneous nickname\r\n", client_fd);
		return ;
	}
	if (this->_nicknames.find(nickname) != this->_nicknames.end())
	{
		_print_error("Nickname already in use", ":" + std::string(SERVERNAME) + " 433 * " + nickname + " :Nickname is already in use\r\n", client_fd);
		return ;
	}

    std::string old_nick = client->nickname;

    client->nickname = nickname;
	// Update the client's nickname
    this->_nicknames[nickname] = client_fd;
	this->_nicknames.erase(old_nick);
	// Update the client's nickname in the channels using Channel's method update_member()
	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		it->second->update_member(old_nick, nickname);
	}

    // Required NICK message for IRC clients to update
    std::string nick_msg = ":" +  old_nick + " NICK :" + nickname + "\r\n";
    if (send(client_fd, nick_msg.c_str(), nick_msg.length(), 0) < 0)
    {
        std::cerr << RED << "SERVER: Failed to send NICK message" << RESET << std::endl;
    }
    else
    {
        std::cout << BLUE << "SERVER: Sent NICK message: " << nick_msg << RESET << std::endl;
    }
}

/**
 * @brief Handles the kick process from a channel
 * 
 * @param channel The channel to kick the user from
 * @param nickname The nickname of the user to kick
 * @param client_fd The file descriptor of the client
 */
void    IRC::_cmd_kick(std::string channel, std::string nickname, std::string comment, int client_fd)
{
	std::map<std::string, Client *> members = this->_channels[channel]->get_members();
	std::map<std::string, Client *> operators = this->_channels[channel]->get_operators();

	//check if nickname exists
	if (this->_nicknames.find(nickname) == this->_nicknames.end())
	{
		_print_error("No such nickname", ":" + std::string(SERVERNAME) + " 401 " + this->_clients[client_fd]->nickname + " " + nickname + " :No such nickname\r\n", client_fd);
		return ;
	}
	//check if nickname isn't the same as the client
	if (this->_clients[client_fd]->nickname == nickname)
	{
		_print_error("Cannot kick yourself", ":" + std::string(SERVERNAME) + " 482 " + this->_clients[client_fd]->nickname + " " + channel + " :Cannot kick yourself\r\n", client_fd);
		return ;
	}
	//kick the user
	std::string kick_comment = comment.empty() ? ":" + nickname + " has been kicked from the channel" : comment;
	std::string kick_msg = ":" + this->_clients[client_fd]->nickname + " KICK " + channel + " " + nickname + " " + kick_comment + "\r\n";

	//send kick message to all members
	this->_channels[channel]->broadcast(kick_msg);
	this->_channels[channel]->remove_member(nickname);
}

/**
 * @brief Handles the topic process for the client
 * 
 * @param channel The channel to change the topic
 * @param topic The new topic
 * @param client_fd The file descriptor of the client
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> TOPIC <channel> :<topic>
 * @note Server format message: :<servername> 482 <channel> :You're not channel operator
 */
void    IRC::_cmd_topic(std::string channel, std::string topic, int client_fd)
{
    if (this->_channels.find(channel) != this->_channels.end())
        this->_channels[channel]->change_topic(this->_clients[client_fd], topic);
    else
    {
		_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + channel + " :No such channel\r\n", client_fd);
    }
}

void    IRC::_cmd_topic(std::string channel, int client_fd)
{
    if (this->_channels.find(channel) != this->_channels.end())
        this->_channels[channel]->get_topic(this->_clients[client_fd]);
    else
    {
		_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + channel + " :No such channel\r\n", client_fd);
    }
}


/**
 * @brief Handles the mode process for the client
 * 
 * @param target The target of the mode change
 * @param mode The mode to change
 * @param client_fd The file descriptor of the client
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> MODE <target> <mode>
 * @note Server format message: :<servername> 482 <target> :You're not channel operator
 */
void    IRC::_cmd_mode(std::string target, std::string mode, int client_fd)
{
    if (this->_channels.find(target) != this->_channels.end())
        this->_channels[target]->change_mode(this->_clients[client_fd], mode);
    else
    {
		_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + target + " :No such channel\r\n", client_fd);
    }
}

/**
 * @brief Handles the mode process for the client
 * 
 * @param target The target of the mode change
 * @param mode The mode to change
 * @param client_fd The file descriptor of the client
 * 
 * @note Client format message: :<nickname>!<username>@<hostname> MODE <target> <mode>
 * @note Server format message: :<servername> 482 <target> :You're not channel operator
 */
void    IRC::_cmd_mode(std::string target, std::string mode, std::string arg, int client_fd)
{
    if (this->_channels.find(target) != this->_channels.end())
        this->_channels[target]->change_mode(this->_clients[client_fd], mode, arg);
    else
    {
		_print_error("No such channel", ":" + std::string(SERVERNAME) + " 403 " + target + " :No such channel\r\n", client_fd);
    }
}

# IRC
## Internet Relay Chat

### IRC Client Comparison

| Feature                   | HexChat                   | Irssi                      | WeeChat                  |
|---------------------------|---------------------------|----------------------------|--------------------------|
| **Interface Type**        | Graphical (GUI)           | Command-line (CLI)         | Command-line (CLI)       |
| **Ease of Use**           | Very high (intuitive GUI) | Moderate (requires CLI skills) | High (user-friendly CLI) |
| **Scripting and Automation** | Limited                 | Extensive (Perl)           | Extensive (multiple languages) |
| **Resource Usage**        | Higher (due to GUI)       | Very low                   | Low                      |
| **Documentation**         | Comprehensive             | Extensive                  | Comprehensive and updated|
| **Customization**         | Moderate                  | High                       | Very high (plugins and scripts) |
| **Community Support**     | Strong                    | Strong                     | Strong                   |
| **Modern Features**       | Yes (GUI-based)           | No                         | Yes (CLI-based)          |

## Usage

### Client

We chose [WeeChat](https://weechat.org/) as client, even though we followed the **IRCv3** Protocol that is compatible with most of the IRC current clients.

#### Add the server in the WeeChat client

```bash
/server add my_irc localhost/6667 -password=MYSECUREPASSWORD
```

> [!NOTE]
> To change the password if needed, we can do that with:
> ```bash
> /set irc.server.my_irc.password CORRECTPASSWORD
> /save
> ```

#### Connect to the IRC server

```bash
/connect my_irc
```

### Network

#### Ports

[Service Name and Transport Protocol Port Number Registry](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml)

> [!NOTE]
> **Internet Assigned Numbers Authority (IANA)**:
> Port numbers are assigned in various ways, based on three ranges: System Ports (0-1023), User Ports (1024-49151), and the Dynamic and/or Private Ports (49152-65535);

| Category | Port Range |
|:--------:|:-----:|
| Well-Known / System | 0 - 1023   |
| Registered / User | 1024 - 49151 | 
| Dynamic / Private | 49152-65535  |

## Development

### Libraries

```C
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
```

### Capabilities

| **Client Message**        | **Description**                                        | **Server Response**                               | **Example Server Response**                                                   |
|---------------------------|--------------------------------------------------------|--------------------------------------------------|------------------------------------------------------------------------------|
| `CAP LS 302`              | Requests list of server capabilities (IRCv3.2)         | List the server capabilities                     | `CAP * LS :multi-prefix sasl`                                                 |
| `CAP LS`                  | Requests list of server capabilities                   | List the server capabilities                     | `CAP * LS :multi-prefix sasl`                                                 |
| `CAP LIST`                | Requests list of currently enabled capabilities        | List the currently enabled capabilities          | `CAP * LIST :multi-prefix`                                                    |
| `CAP REQ :<capability>`   | Requests to enable specific capabilities               | Acknowledge or deny the capability request       | `CAP * ACK :multi-prefix` (if accepted)                                       |
|                           |                                                        |                                                  | `CAP * NAK :multi-prefix` (if rejected)                                       |
| `CAP END`                 | Ends capability negotiation                            | No response needed                               | N/A                                                                          |
| `CAP ABORT`               | Aborts the capability negotiation                      | No response needed                               | N/A                                                                          |
| `CAP CLEAR`               | Requests to disable all enabled capabilities           | Acknowledge the request                          | `CAP * ACK :`                                                                 |
| `CAP NEW :<capability>`   | Informs the client of new capabilities after registration| List the new capabilities                        | `CAP * NEW :extended-join`                                                    |
| `CAP DEL :<capability>`   | Informs the client of removed capabilities after registration| List the removed capabilities                    | `CAP * DEL :extended-join`                                                    |

### Example Server Responses

1. **`CAP LS 302` or `CAP LS`:**
   ```text
   CAP * LS :multi-prefix sasl
   ```

2. **`CAP LIST`:**
   ```text
   CAP * LIST :multi-prefix
   ```

3. **`CAP REQ :multi-prefix`:**
   - If accepted:
     ```text
     CAP * ACK :multi-prefix
     ```
   - If rejected:
     ```text
     CAP * NAK :multi-prefix
     ```

4. **`CAP END`:**
   - No direct response required.

5. **`CAP ABORT`:**
   - No direct response required.

6. **`CAP CLEAR`:**
   ```text
   CAP * ACK :
   ```

7. **`CAP NEW :extended-join`:**
   ```text
   CAP * NEW :extended-join
   ```

8. **`CAP DEL :extended-join`:**
   ```text
   CAP * DEL :extended-join
   ```

### Summary

- **`CAP LS 302` and `CAP LS`:** Respond with the list of server capabilities.
- **`CAP LIST`:** Respond with the list of currently enabled capabilities.
- **`CAP REQ :<capability>`:** Acknowledge or deny the requested capabilities.
- **`CAP END` and `CAP ABORT`:** No response required.
- **`CAP CLEAR`:** Acknowledge the request to disable all capabilities.
- **`CAP NEW :<capability>` and `CAP DEL :<capability>`:** Inform the client of new or removed capabilities after registration.

### IRCv3 Communication Commands


| Command | Parameters                          | Description                                                      | Example                                                       | Expected Response                                                                 |
|---------|-------------------------------------|------------------------------------------------------------------|---------------------------------------------------------------|-----------------------------------------------------------------------------------|
| PASS    | `<password>`                        | Sets a connection password                                       | `PASS secretpassword`                                         | No response if successful. `ERROR` message if failed.                             |
| CAP     | `<subcommand> [:<capabilities>]`    | Requests or lists supported capabilities                         | `CAP REQ :multi-prefix`                                       | `CAP` acknowledgment or error message.                                            |
| NICK    | `<nickname>`                        | Sets or changes the user's nickname                              | `NICK newnickname`                                            | `NICK` change confirmation or error if nickname is invalid or in use.             |
| USER    | `<username> 0 * <realname>`         | Registers a new user with the server                             | `USER myusername 0 * My Real Name`                            | Welcome messages from the server, including `001` (RPL_WELCOME).                  |
| QUIT    | `[<reason>]`                        | Disconnects from the server, optionally providing a reason       | `QUIT :Goodbye everyone`                                      | No response from the server as the user is disconnected.                          |
| JOIN    | `<channel>{,<channel>} [<key>{,<key>}]` <br> Alt Params: `0` | Joins one or more channels, optionally with keys <br> Leaves all channels | `JOIN #channel1,#channel2 key1,key2` <br> `JOIN 0` | `JOIN` confirmation and `MODE`, `TOPIC`, and `NAMES` responses for each channel. |
| TOPIC   | `<channel> [<topic>]`               | Sets or views the topic of a channel                             | `TOPIC #channel :New Topic`                                   | `TOPIC` confirmation or the current topic if no new topic is provided.            |
| KICK    | `<channel> <user> *( "," <user> ) [<comment>]` | Removes one or more users from a channel, optionally with a comment | `KICK #channel user1,user2 :Breaking rules`                   | `KICK` message for each user kicked.                                              |
| INVITE  | `<nickname> <channel>`              | Invites a user to a channel                                      | `INVITE friend #channel`                                      | `INVITE` confirmation and notification to the invited user.                       |
| MODE    | `<target> [<modestring> [<mode arguments>...]]` | Changes modes for a user or channel                              | `MODE #channel +o username`                                    | `MODE` confirmation message.                                                      |

- JOIN: Can be used from anywhere in the IRC client to join channels.
- TOPIC: Must be used from within the channel; usually only operators can change it.
- KICK: Must be executed from within the channel; only operators can kick users.
- INVITE: Can be executed from anywhere, specifying the user and channel.
- MODE: Must be used from within the channel; typically requires operator privileges.

#### MODE Command Options

| Option | Description                                   | Example                                | Expected Response                  |
|--------|-----------------------------------------------|----------------------------------------|------------------------------------|
| +i     | Sets the channel to invite-only mode          | `MODE #channel +i`                     | `MODE #channel +i` confirmation    |
| +t     | Only channel operators can set the topic      | `MODE #channel +t`                     | `MODE #channel +t` confirmation    |
| +k     | Sets a key (password) for the channel         | `MODE #channel +k secretkey`           | `MODE #channel +k` confirmation    |
| +o     | Gives operator status to a user               | `MODE #channel +o username`            | `MODE #channel +o` confirmation    |
| +l     | Sets a user limit on the channel              | `MODE #channel +l 50`                  | `MODE #channel +l` confirmation    |


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

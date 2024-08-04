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
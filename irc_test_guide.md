
# 🧪 IRC Server Testing Guide (WeeChat + Two Terminals)

This guide helps you test your `ft_irc` server using **two WeeChat clients** running in separate terminals.

---

## 🖥 Setup

- **Server address:** `127.0.0.1`
- **Port:** `6667`
- **Password:** `password123`
- **Clients:** Two WeeChat instances running in different terminals

---

## ✅ Step 1: Connect to Your IRC Server

Run these commands in both terminals:

```bash
/weechat
/server add localirc 127.0.0.1/6667 -password=password123
/connect localirc
```

---

## ✅ Step 2: Set Nicknames and Usernames

### Terminal 1 (Client A):
```bash
/nick Alice
/set irc.server.localirc.username alice
/set irc.server.localirc.realname "Alice Tester"
```

### Terminal 2 (Client B):
```bash
/nick Bob
/set irc.server.localirc.username bob
/set irc.server.localirc.realname "Bob Tester"
```

---

## ✅ Step 3: Join a Channel

### Terminal 1 (Alice):
```bash
/join #testchannel
```

### Terminal 2 (Bob):
```bash
/join #testchannel
```

---

## ✅ Step 4: Test Channel Messaging

### Alice (in Terminal 1):
```bash
Hello Bob!
```

Bob should see the message in Terminal 2.

---

## ✅ Step 5: Private Message (PRIVMSG)

### Bob (in Terminal 2):
```bash
/msg Alice hey!
```

Alice should receive a private message.

---

## ✅ Step 6: Channel Modes (MODE)

### Alice (channel operator) sets and unsets modes:

```bash
/mode #testchannel +i         # Invite-only
/mode #testchannel -i

/mode #testchannel +k secret  # Channel key (password)
/mode #testchannel -k

/mode #testchannel +l 2       # User limit
/mode #testchannel -l

/mode #testchannel +t         # Only ops can change topic
/mode #testchannel -t
```

---

## ✅ Step 7: Topic Command

### Alice:
```bash
/topic #testchannel Testing topic change
```

Bob should be blocked if `+t` is active.

---

## ✅ Step 8: Kick, Invite, Op Privileges

### Alice kicks Bob:
```bash
/kick #testchannel Bob Bye!
```

### Alice invites Bob back:
```bash
/invite Bob #testchannel
```

### Alice gives operator to Bob:
```bash
/mode #testchannel +o Bob
```

### Alice removes operator from Bob:
```bash
/mode #testchannel -o Bob
```

---

## ✅ Step 9: Quit

```bash
/quit Leaving test
```

---

## 🔬 Extra: Test Partial Commands (Optional)

Use `nc` to simulate partial TCP packets:

```bash
nc -C 127.0.0.1 6667
```

Then type and press `Ctrl+D` after each part:

```
N
^D
ICK
^D
 Alice
^D
```

---

Happy testing! 🎉

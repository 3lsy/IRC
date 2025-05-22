#!/bin/bash

# Usage: ./weechat.sh <nickname> <port> <password>

NICKNAME="$1"
PORT="$2"
PASSWORD="$3"

# Basic validation
if [[ -z "$NICKNAME" || -z "$PORT" || -z "$PASSWORD" ]]; then
  echo "Usage: $0 <nickname> <port> <password>"
  exit 1
fi

CUID=$(head /dev/urandom | tr -dc 'a-f0-9' | head -c 6)

docker run -it --rm --name "weechat-${NICKNAME}-${CUID}" --network host weechat/weechat:3.8 bash -c "
  sleep 2 && weechat -r '/server add myserver localhost/$PORT -notls' \
          -r '/set irc.server.myserver.nicks $NICKNAME' \
          -r '/set irc.server.myserver.username $NICKNAME' \
          -r '/set irc.server.myserver.realname Dockerized-WeeChat' \
          -r '/set irc.server.myserver.password $PASSWORD' \
          -r '/set irc.server.myserver.autoconnect on' \
          -r '/connect myserver'
"

echo "Weechat launched with nickname $NICKNAME, connecting to localhost:$PORT with password $PASSWORD"


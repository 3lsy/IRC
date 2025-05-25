#!/bin/bash

# Usage: ./weechat.sh <nickname> <host> <port> <password>

NICKNAME="$1"
HOST="$2"
PORT="$3"
PASSWORD="$4"

# Basic validation
if [[ -z "$NICKNAME" || -z "$HOST" || -z "$PORT" || -z "$PASSWORD" ]]; then
  echo "Usage: $0 <nickname> <host> <port> <password>"
  exit 1
fi

CUID=$(head /dev/urandom | tr -dc 'a-f0-9' | head -c 6)

docker run -it --rm --name "weechat-${NICKNAME}-${CUID}" --network host weechat/weechat:3.8 bash -c "
  weechat -r '/server add myserver ${HOST}/${PORT} -notls' \
          -r '/set irc.server.myserver.nicks $NICKNAME' \
          -r '/set irc.server.myserver.username $NICKNAME' \
          -r '/set irc.server.myserver.realname Dockerized-WeeChat' \
          -r '/set irc.server.myserver.password $PASSWORD' \
          -r '/set irc.server.myserver.autoconnect on' \
          -r '/connect myserver'
"

echo "Weechat launched with nickname $NICKNAME, connecting to $HOST:$PORT with password $PASSWORD"

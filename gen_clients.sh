#!/bin/bash

# Check if N (number of directories) is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_directories>"
    exit 1
fi

# Number of directories to create
N=$1

# Base directory for temporary directories
BASE_DIR="/tmp"

# Generate N directories with unique IDs and launch weechat in each
for i in $(seq 1 $N); do
    # Generate a unique directory name
    UNIQUE_DIR=$(mktemp -d "${BASE_DIR}/XXXXXX")

    # Create a unique nickname based on the directory name
    NICKNAME=$(basename "$UNIQUE_DIR")

    # Open a terminal and start Weechat with the specified directory, nickname, and server connection setup
    gnome-terminal -- bash -c "weechat --dir $UNIQUE_DIR \
        --run-command '/server add my_irc localhost/6667 -password=password' \
        --run-command '/wait 1' \
        --run-command '/set irc.server.my_irc.autoconnect on' \
        --run-command '/set irc.server.my_irc.nicks \"$NICKNAME\"' \
        --run-command '/connect my_irc'"

    echo "Launched Weechat in directory $UNIQUE_DIR with nickname $NICKNAME, connecting to my_irc on localhost:6667"
done

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: echavez- <echavez-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/27 11:58:01 by echavez-          #+#    #+#              #
#    Updated: 2024/08/11 14:16:16 by echavez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        =   ircserv                              # Name of the binary

#****************** INC *******************#
# General
INC         =   ./include/                           # Project header files drectory

INCLUDE     =   -O3 -I $(INC)            			 # Header files

#****************** SRC *******************#

DIRSRC      =   ./src/
DIRSRV  	:=  $(DIRSRC)/server/
DIRUSR      :=  $(DIRSRC)/users/
DIRCOM	   	:=  $(DIRSRC)/communication/

DIRS        :=  $(DIRSRC) $(DIRSRV) $(DIRUSR) $(DIRCOM)

SRC         =   main.cpp #debug.c remove debug.c when done
SRV			=	IRC.cpp IRC_socket.cpp IRC_serve.cpp IRC_connection.cpp
#SOC         =   socket.cpp
USR         =   Client.cpp
COM         =   Client_commands.cpp

SRCS        :=  $(SRC) $(SRV) $(USR) $(COM) #$(SOC)

#***************** DEPS ******************#

DIROBJ      =   ./depo/

#********************************* END OF CONFIG *********************************#

OAUX        =   $(SRCS:%=$(DIROBJ)%)
DEPS        =   $(OAUX:.cpp=.d)
OBJS        =   $(OAUX:.cpp=.o)

.ONESHELL:

$(info Creating directory...)
$(shell mkdir -p $(DIROBJ))

ifdef FLAGS
    ifeq ($(FLAGS), no)
CFLAGS      =
    endif
    ifeq ($(FLAGS), debug)
CFLAGS      =   -Wall -Wextra -Werror -std=c++98 -ansi -pedantic -g
    endif
else
CFLAGS      =   -Wall -Wextra -Werror -std=c++98
endif

ifdef VERB
    ifeq ($(VERB), on)
CFLAGS      +=  -DM_VERB
    endif
endif

ifndef VERBOSE
.SILENT:
endif

ENV         =   /usr/bin/env
CC          =   $(ENV) clang++
RM          =   $(ENV) rm -rf
ECHO        =   $(ENV) echo -e
MKDIR       =   $(ENV) mkdir -p
GIT         =   $(ENV) git
BOLD        =   "\e[1m"
REVER       =   "\e[7m"
BLINK       =   "\e[5m"
RED         =   "\e[38;2;255;0;0m"
GREEN       =   "\e[92m"
BLUE        =   "\e[34m"
YELLOW      =   "\e[33m"
E0M         =   "\e[0m"

FMT         :=  $(REVER)$(YELLOW)$(BLINK)

#******************************* DEPS COMPILATION ********************************#

define generate_objects
$(DIROBJ)%.o    :   $(1)%.cpp
	                @printf $(GREEN)"Generating $(NAME) objects... %-33.33s\r"$(E0M) $$@
	                @$(CC) $(CFLAGS) $(INCLUDE) -MMD -o $$@ -c $$<
endef

# Generate objects
$(foreach dir,$(DIRS),$(eval $(call generate_objects,$(dir))))

#******************************* MAIN COMPILATION ********************************#

$(NAME)         :   $(OBJS)
	                @$(CC) $(INCLUDE) $(CFLAGS) -o $(NAME) $(OBJS)
	                @$(ECHO)
	                @$(ECHO) '                       '$(FMT)'██████'$(E0M)
	                @$(ECHO) '                   '$(FMT)'████░░░░░░██'$(E0M)
	                @$(ECHO) '                 '$(FMT)'██░░░░░░░░██'$(E0M)
	                @$(ECHO) '               '$(FMT)'██░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '             '$(FMT)'██░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '           '$(FMT)'██░░░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '           '$(FMT)'██░░░░░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '   '$(FMT)'████████░░░░░░░░░░░░░░░░░░░░░░░░████████'$(E0M)
	                @$(ECHO) ' '$(FMT)'██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) ' '$(FMT)'██▓▓░░░░░░░░░░░░  ██░░░░  ██░░░░░░░░░░░░▓▓██'$(E0M)
	                @$(ECHO) '   '$(FMT)'██▓▓░░░░░░░░░░████░░░░████░░░░░░░░░░▓▓██'$(E0M)
	                @$(ECHO) '     '$(FMT)'██▓▓░░░░░░░░████░░░░████░░░░░░░░▓▓██'$(E0M)
	                @$(ECHO) '       '$(FMT)'██░░░░░░░░▓▓██░░░░██▓▓░░░░░░░░██'$(E0M)
	                @$(ECHO) '       '$(FMT)'██░░░░░░░░░░░░░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '       '$(FMT)'██░░░░░░░░░░░░░░░░░░░░░░░░░░░░██'$(E0M)
	                @$(ECHO) '       '$(FMT)'██▓▓░░░░░░░░░░░░░░░░░░░░░░░░▓▓██'$(E0M)
	                @$(ECHO) '       '$(FMT)'██▓▓░░░░░░░░░░░░░░░░░░░░░░░░▓▓██'$(E0M)
	                @$(ECHO) '         '$(FMT)'██░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░██'$(E0M)
	                @$(ECHO) '         '$(FMT)'██▓▓░░▓▓████████████▓▓░░▓▓██'$(E0M)
	                @$(ECHO) '           '$(FMT)'██▓▓██'$(E0M)'            '$(FMT)'██▓▓██'$(E0M)
	                @$(ECHO) '             '$(FMT)'██'$(E0M)'                '$(FMT)'██'$(E0M)
	                @$(ECHO) $(E0M)
	                @$(ECHO) $(BOLD)$(GREEN)'> '$(NAME)' Compiled'$(E0M)

$(OBJS): | mkdepo

clean           :
	                @($(RM) $(OBJS))
	                @($(RM) $(DEPS))
	                @($(RM) $(DIROBJ))
	                @$(ECHO) $(BOLD)$(RED)'> '$(NAME)' directory        cleaned'$(E0M)

all             :       $(NAME)

fclean          :       clean
	                @$(RM) $(NAME)
	                @$(ECHO) $(BOLD)$(RED)'> Executable             removed'$(E0M)

re              :       fclean mkdepo all

mkdepo			:
					@$(MKDIR) $(DIROBJ)

init            :
	                @$(GIT) submodule init
	                @$(GIT) submodule update

.PHONY          :       all clean re fclean mkdepo

-include $(DEPS)

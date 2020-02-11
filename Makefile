# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/25 16:38:11 by wbraeckm          #+#    #+#              #
#    Updated: 2020/02/11 16:22:16 by wbraeckm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
FLAGS = -O3 -Wall -Wextra -Werror
INCLUDES = ./includes/
SRCSFOLDER = ./srcs/
OBJFOLDER = ./obj/
.DEFAULT_GOAL = all

ITEMS = $(shell find srcs -type f | grep -E "\.c$$" | sed 's/srcs//g')
SRCS = $(addprefix $(SRCSFOLDER), $(ITEMS))
HEADERS = $(shell find includes -type f)
OBJ = $(addprefix $(OBJFOLDER), $(ITEMS:.c=.o))
SRCSUBS = $(shell find ./srcs -type d)
OBJSUBS = $(SRCSUBS:./srcs%=./obj%)
LONGEST = $(shell echo $(notdir $(SRCS)) | tr " " "\n" | \
	awk ' { if ( length > x ) { x = length; y = $$0 } }END{ print y }' | wc -c \
	| sed 's/ //g')

CC_EOC = \e[0m
CC_BOLD = \e[1m
CC_DIM = \e[2m
CC_UNDERLINE = \e[4m
CC_BLINK = \e[5m
CC_INVERT = \e[7m
CC_HIDDEN = \e[8m
CC_DEF = \e[39m
CC_BLACK = \e[30m
CC_RED = \e[31m
CC_GREEN = \e[32m
CC_YELLOW = \e[33m
CC_BLUE = \e[34m
CC_MAGENTA = \e[35m
CC_CYAN = \e[36m
CC_GRAY = \e[37m
CC_DARK_GRAY = \e[90m
CC_LIGHT_RED = \e[91m
CC_LIGHT_GREEN = \e[92m
CC_LIGHT_YELLOW = \e[93m
CC_LIGHT_BLUE = \e[94m
CC_LIGHT_MAGENTA = \e[95m
CC_LIGHT_CYAN = \e[96m
CC_WHITE = \e[97m
CC_BG_DEF = \e[49m
CC_BG_BLACK = \e[40m
CC_BG_RED = \e[41m
CC_BG_GREEN = \e[42m
CC_BG_YELLOW = \e[43m
CC_BG_BLUE = \e[44m
CC_BG_MAGENTA = \e[45m
CC_BG_CYAN = \e[46m
CC_BG_GRAY = \e[47m
CC_BG_DARK_GRAY = \e[100m
CC_BG_LIGHT_RED = \e[101m
CC_BG_LIGHT_GREEN = \e[102m
CC_BG_LIGHT_YEWLLO = \e[103m
CC_BG_LIGHT_BLUE = \e[104m
CC_BG_LIGHT_MAGENTA = \e[105m
CC_BG_LIGHT_CYAN = \e[106m
CC_BG_WHITE = \e[107m

all: $(NAME)

$(OBJFOLDER)/%.o:$(SRCSFOLDER)/%.c $(HEADERS) $(LIBFT_HEADERS) Makefile
	@printf "\r$(CC_BLUE)Compiling %-$(LONGEST)s" $(notdir $<)
	@$(CC) $(FLAGS) -fPIC -o $@ -c $< -I$(INCLUDES)
	@printf "$(CC_GRAY)[$(CC_LIGHT_GREEN)√$(CC_GRAY)]$(CC_EOC)"

$(OBJSUBS):
	@mkdir $@

$(NAME): $(OBJSUBS) $(OBJ)
	@printf "\r$(CC_LIGHT_CYAN)Compiling $(NAME) "
	@$(CC) $(FLAGS) -shared -o $(NAME) $(OBJ) -I$(INCLUDES)
	@rm -f libft_malloc.so
	@ln -s $(NAME) libft_malloc.so
	@printf "$(CC_GRAY)[$(CC_LIGHT_GREEN)√$(CC_GRAY)]$(CC_EOC)"
	@printf "                                                     \n"

clean:
	@printf "$(CC_RED)"
	rm -rf $(OBJFOLDER) $(LIBFT_OBJFOLDER)
	@printf "$(CC_EOC)"

fclean: clean
	@printf "$(CC_RED)"
	rm -rf $(NAME) $(LIBFT)
	@printf "$(CC_EOC)"

re: fclean all

norm:
	@norminette $(SRCSFOLDER) $(INCLUDES)

.PHONY: norm clean fclean re

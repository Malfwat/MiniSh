# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/09 18:24:33 by malfwa            #+#    #+#              #
#    Updated: 2025/06/23 09:55:00 by malfwa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

SRC	=	main.c

SRC +=	gnl/get_next_line_utils.c\
		gnl/get_next_line.c\
		gnl/get_next_null.c\
		gnl/gnl.c\
		gnl/gnl_utils.c\
		gnl/utils.c

SRC	+=	history/manage_history.c

SRC	+=	signal/handler.c

SRC	+=	utils/utils.c\
		utils/sep_func.c

SRC	+=	parsing/readline.c\
		parsing/hash.c\
		parsing/tilde.c\
		parsing/manage_rcfile.c\
		parsing/parse_rc.c\
		parsing/snippet.c\
		parsing/word_len.c\
		parsing/hash_utils.c\
		parsing/syntaxe/check_syntaxe.c\
		parsing/syntaxe/alias.c\
		parsing/wildcard/wildcard.c\
		parsing/wildcard/manage_files.c

SRC	+=	prompt/generate_prompt.c\
		prompt/write_prompt.c

CC	=	cc

CFLAGS	=	-Wall -Wextra -Werror -MMD -g3
LDFLAGS	=	-lreadline -lncurses

INCLUDES	=	includes/ 
INCLUDES	+=	-I srcs/printf/includes
INCLUDES	+=	-I srcs/printf/libft

SRC_DIR	=	srcs/

BUILD	=	.build/

OBJ	=	$(addprefix $(BUILD), $(SRC:.c=.o))

DEPS	=	$(OBJ:.o=.d)

LIBPRINTF	=	srcs/printf/libftprintf.a

all:	$(NAME)

$(LIBPRINTF):
	make bonus -C srcs/printf

$(NAME):	$(LIBPRINTF) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) -L/srcs/printf $(LIBPRINTF)

$(BUILD)%.o:	$(SRC_DIR)%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDES)

clean:
	rm -rf $(BUILD)
	rm -rf srcs/printf/.build

fclean:	clean
	rm -rf $(NAME)
	make fclean -C srcs/printf

re: fclean all

.PHONY: re all fclean clean

-include $(OBJ:.o=.d)


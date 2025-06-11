# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/09 18:24:33 by malfwa            #+#    #+#              #
#    Updated: 2025/06/09 22:00:22 by malfwa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

SRC	=	main.c

SRC +=	gnl/get_next_line_utils.c\
		gnl/get_next_line.c\
		gnl/gnl.c\
		gnl/utils.c

SRC	+=	history/manage_history.c

SRC	+=	signal/handler.c

SRC	+=	utils/utils.c

SRC	+=	parsing/readline.c\
		parsing/hash.c\
		parsing/hash_utils.c

SRC	+=	prompt/generate_prompt.c

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
	make -C srcs/printf

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


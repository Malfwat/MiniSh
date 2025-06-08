# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: admoufle <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 13:09:01 by admoufle          #+#    #+#              #
#    Updated: 2025/04/29 10:39:02 by admoufle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	../.build/libft.a

################################################################################
#                                                                              #
#                                                                              #
#                                  VARIABLES                                   #
#                                                                              #
#                                                                              #
################################################################################

SRC_DIR		=	./


SRC			=	ft_atoi.c		\
				ft_bzero.c		\
				ft_calloc.c		\
				ft_isalnum.c	\
				ft_isalpha.c	\
				ft_isascii.c	\
				ft_isdigit.c	\
				ft_isprint.c	\
				ft_itoa.c		\
				ft_memchr.c		\
				ft_memcmp.c		\
				ft_memcpy.c		\
				ft_memmove.c	\
				ft_memset.c		\
				ft_putchar_fd.c	\
				ft_putstr_fd.c	\
				ft_putendl_fd.c	\
				ft_putnbr_fd.c	\
				ft_strchr.c		\
				ft_strlcat.c	\
				ft_striteri.c	\
				ft_strjoin.c	\
				ft_strsjoin.c	\
				ft_strlcpy.c	\
				ft_strlen.c		\
				ft_split.c		\
				ft_strmapi.c	\
				ft_strdup.c		\
				ft_strncmp.c	\
				ft_strnstr.c	\
				ft_strchr.c		\
				ft_strtrim.c	\
				ft_strrchr.c	\
				ft_substr.c		\
				ft_tolower.c	\
				ft_toupper.c

SRC_BONUS	=	ft_lstnew.c			\
				ft_lstsize.c		\
				ft_lstlast.c		\
				ft_lstadd_front.c	\
				ft_lstadd_back.c	\
				ft_lstdelone.c		\
				ft_lstclear.c		\
				ft_lstiter.c		\
				ft_lstmap.c

OBJ			=	$(addprefix $(OBJ_DIR), $(SRC:.c=.o))

OBJ_BONUS	=	$(addprefix $(OBJ_DIR), $(SRC_BONUS:.c=.o))

DEPS		=	$(OBJ:.o=.d)
DEPS		+=	$(OBJ_BONUS:.o=.d)

LC			=	ar rc

CC			=	cc

CFLAGS		+= -Wall -Werror -Wextra -MMD -MP

OBJ_DIR		=	.build/

INC			=	-I./

################################################################################
#                                                                              #
#                                                                              #
#                                    RULES                                     #
#                                                                              #
#                                                                              #
################################################################################

all:	$(NAME)
	
$(OBJ_DIR):
	mkdir .build

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)


$(NAME):	$(OBJ_DIR) $(OBJ)
	$(LC) $(NAME) $(OBJ)

bonus:	$(OBJ_DIR) $(OBJ_BONUS) $(OBJ)
	$(LC) $(NAME) $(OBJ) $(OBJ_BONUS)

bonusp:	$(OBJ_DIR) $(OBJ_BONUS) $(OBJ)
	$(LC) -o ../.build/libft_bonus.a $(OBJ) $(OBJ_BONUS)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY: bonus clean fclean all re

-include $(DEPS)

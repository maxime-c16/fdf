# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/09 17:39:45 by mcauchy           #+#    #+#              #
#    Updated: 2025/03/19 13:58:14 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	ft_atoi.c ft_isascii.c ft_memccpy.c ft_memset.c ft_strlcpy.c ft_strrchr.c \
					ft_bzero.c ft_isdigit.c ft_memchr.c ft_putchar_fd.c ft_strchr.c ft_strlen.c ft_strtrim.c \
					ft_calloc.c ft_isprint.c ft_memcmp.c  ft_putendl_fd.c ft_strdup.c  ft_strmapi.c ft_substr.c \
					ft_isalnum.c ft_itoa.c ft_memcpy.c ft_putnbr_fd.c ft_strjoin.c ft_strncmp.c ft_tolower.c \
					ft_isalpha.c ft_memmove.c ft_putstr_fd.c ft_strlcat.c ft_strnstr.c ft_toupper.c ft_strcmp.c \
					ft_strcpy.c ft_str_is_numeric.c ft_split.c ft_striteri.c get_next_line.c get_next_line_utils.c \

BONUS			=	ft_lstnew.c ft_lstadd_back.c ft_lstadd_front.c ft_lstlast.c ft_lstsize.c ft_lstdelone.c \
					ft_lstclear.c ft_lstiter.c ft_lstmap.c

BONUS_OBJS		=	$(addprefix $(DIR_OBJ)/, $(BONUS:.c=.o))

DIR_OBJ			=	../.objs

NAME			=	libft.a

OBJ				=	$(addprefix $(DIR_OBJ)/, $(SRCS:.c=.o))

CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror -g3

HEADER			=	libft.h

all				:	$(NAME)

$(NAME)			:	$(OBJ) $(BONUS_OBJS)
					@echo "Creating libft.a"
					@ar rc $@ $^
					@ranlib $@

$(DIR_OBJ)/%.o	:	%.c $(HEADER)
					@mkdir -p $(@D)
					@echo "Compiling $(notdir $<)..."
					@$(CC) $(CFLAGS) -o $@ -c $<

clean			:
					@/bin/rm -f $(OBJ)
					@/bin/rm -rf $(DIR_OBJ)
					@/bin/rm -f $(BONUS_OBJS)

fclean			:	clean
					@/bin/rm -f $(NAME)

re				:	fclean all

.PHONY			:	all clean norme fclean re

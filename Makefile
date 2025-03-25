# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 14:49:38 by macauchy          #+#    #+#              #
#    Updated: 2025/03/24 15:22:36 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	=	fdf.c singleton.c parsing.c projection.c gyro.c hooks.c port_audio.c
SRC_DIR	=	srcs
SRCS	=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJ_DIR	=	.objs
OBJS	=	$(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

NAME	=	fdf
CC		=	gcc
CFLAGS	=	-g3
DEBUG	=	-fsanitize=address
RM		=	/bin/rm -rf
LDFLAGS	=	-Llibft -lft -Lmlx -lmlx -lft_gl -Lft_gl -framework OpenGL \
			-framework AppKit -fsanitize=address -lportaudio

all:		$(NAME)

$(NAME):	make_libs $(OBJS)
		@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
		@echo "Usage: ./$(NAME) <map>.fdf"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
		@echo "Compiling $<"
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@

debug:		$(OBJS)
		$(MAKE) -C libft -j > /dev/null 2>&1
		$(MAKE) -C mlx -j > /dev/null 2>&1
		$(CC) $(CFLAGS) $(DEBUG) $(OBJS) $(LDFLAGS) -o $(NAME)

make_libs:
		@$(MAKE) -C libft -q || (echo "Compiling libft" && $(MAKE) -C libft -j > /dev/null 2>&1)
		@$(MAKE) -C mlx -q || (echo "Compiling mlx" && $(MAKE) -C mlx -j > /dev/null 2>&1)
		@$(MAKE) -C ft_gl -q || (echo "Compiling ft_gl" && $(MAKE) -C ft_gl -j > /dev/null 2>&1)

clean:
		@echo "Cleaning $(NAME)"
		@$(MAKE) -C libft clean -j > /dev/null 2>&1
		@$(MAKE) -C mlx clean -j > /dev/null 2>&1
		@$(MAKE) -C ft_gl clean -j > /dev/null 2>&1
		@$(RM) $(OBJ_DIR)

fclean: clean
		@$(MAKE) -C libft fclean -j > /dev/null 2>&1
		@$(MAKE) -C ft_gl fclean -j > /dev/null 2>&1
		@$(RM) $(NAME)


re:			fclean all

.PHONY:		all clean fclean re

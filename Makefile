# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 14:49:38 by macauchy          #+#    #+#              #
#    Updated: 2025/05/22 11:29:13 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	=	fdf.c singleton.c parsing.c hooks.c free.c	projection_iso.c			\
			color_utils.c draw_map.c event_close.c line_utils.c pixel_utils.c		\
			gyro_draw.c gyro_line.c gyro_point.c parsing_map.c parsing_utils.c		\
			parsing_print.c draw_map_utils.c draw_map_lines.c draw_map_str.c		\
			event_keys_helpers.c event_keys.c projection_point.c projection_styles.c\
			projection_rotation.c
SRC_DIR	=	srcs
SRCS	=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJ_DIR	=	.objs
OBJS	=	$(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))
HEADER	=	includes/fdf.h
NAME	=	fdf
CC		=	gcc
CFLAGS	=	-g3
DEBUG	=	-fsanitize=address
RM		=	/bin/rm -rf
LDFLAGS	=	-Llibft -lft -Lmlx -lmlx -lX11 -lXext -lm

all:		$(NAME)

$(NAME):	make_libs $(OBJS)
		@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
		@echo "Usage: ./$(NAME) <map>.fdf"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER)
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

clean:
		@echo "Cleaning $(NAME)"
		@$(MAKE) -C libft clean -j > /dev/null 2>&1
		@$(MAKE) -C mlx clean -j > /dev/null 2>&1
		@$(RM) $(OBJ_DIR)

fclean: clean
		@$(MAKE) -C libft fclean -j > /dev/null 2>&1
		@$(RM) $(NAME)


re:			fclean all

.PHONY:		all clean fclean re

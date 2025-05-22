# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 14:49:38 by macauchy          #+#    #+#              #
#    Updated: 2025/05/22 15:46:44 by macauchy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET	= \033[0m
BOLD	= \033[1m
RED		= \033[31m
GREEN	= \033[32m
YELLOW	= \033[33m
BLUE	= \033[34m
CYAN	= \033[36m

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
CC		=	cc
CFLAGS	=	-g3 -Wall -Wextra -Werror
DEBUG	=	-fsanitize=address
RM		=	/bin/rm -rf
LDFLAGS	=	-Llibft -lft -Lmlx -lmlx -lX11 -lXext -lm

all:		$(NAME)

$(NAME):	make_libs $(OBJS)
		@echo "$(BOLD)$(CYAN)[Link]$(RESET) $(NAME)"
		@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
		@echo "$(GREEN)Build complete.$(RESET)"
		@echo "$(BOLD)Usage: $(CYAN)./$(NAME) <map>.fdf$(RESET)"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER)
		@echo "$(YELLOW)[Compiling]$(RESET) $<"
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@

debug:		$(OBJS)
		@echo "$(BOLD)$(CYAN)[Debug Build]$(RESET)"
		@$(MAKE) -C libft -j > /dev/null 2>&1
		@$(MAKE) -C mlx -j > /dev/null 2>&1
		@$(CC) $(CFLAGS) $(DEBUG) $(OBJS) $(LDFLAGS) -o $(NAME)
		@echo "$(GREEN)Debug build complete.$(RESET)"

make_libs:
		@$(MAKE) -C libft -q || (echo "$(BLUE)[Libft]$(RESET) Compiling libft" && $(MAKE) -C libft -j > /dev/null 2>&1)
		@$(MAKE) -C mlx -q || (echo "$(BLUE)[MLX]$(RESET) Compiling mlx" && $(MAKE) -C mlx -j > /dev/null 2>&1)

clean:
		@echo "$(RED)[Cleaning]$(RESET) $(NAME)"
		@$(MAKE) -C libft clean -j > /dev/null 2>&1
		@$(MAKE) -C mlx clean -j > /dev/null 2>&1
		@$(RM) $(OBJ_DIR)
		@echo "$(GREEN)Clean complete.$(RESET)"

fclean: clean
		@$(MAKE) -C libft fclean -j > /dev/null 2>&1
		@$(RM) $(NAME)
		@echo "$(GREEN)Full clean complete.$(RESET)"

re:			fclean all

.PHONY:		all clean fclean re

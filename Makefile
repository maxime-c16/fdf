FILES	=	fdf.c
SRC_DIR	=	srcs
SRCS	=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJ_DIR	=	.objs
OBJS	=	$(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

NAME	=	fdf
CC		=	gcc
CFLAGS	=	-g3
DEBUG	=	-fsanitize=address
RM		=	/bin/rm -rf
LDFLAGS	=	 -Llibft -lft -Lmlx -lmlx -framework OpenGL -framework AppKit

all:		$(NAME)

$(NAME):	$(OBJS)
		@$(MAKE) -C libft -j > /dev/null 2>&1
		@echo "libft compiled"
		@$(MAKE) -C mlx -j > /dev/null 2>&1
		@echo "mlx compiled"
		@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
		@echo "Compiled $(NAME)"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo "Compiled $<"

debug:		$(OBJS)
		$(MAKE) -C libft -j > /dev/null 2>&1
		$(MAKE) -C mlx -j > /dev/null 2>&1
		$(CC) $(CFLAGS) $(DEBUG) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
		@$(MAKE) -C libft clean -j > /dev/null 2>&1
		@$(MAKE) -C mlx clean -j > /dev/null 2>&1
		@$(RM) $(OBJ_DIR)
		@echo "Cleaned $(NAME)"

fclean: clean
		@$(MAKE) -C libft fclean -j > /dev/null 2>&1
		@$(RM) $(NAME)


re:			fclean all

.PHONY:		all clean fclean re

NAME       := pipex
CC         := cc
CFLAGS     := -Wall -Wextra -Werror -g
INCLUDES   := -I include -I libft

SRC_DIR    := src
SRCS       := $(addprefix $(SRC_DIR)/, main.c exec_cmd.c pipex.c error_exit.c)
OBJS       := $(SRCS:.c=.o)

LIBFT_DIR  := libft
LIBFT      := $(LIBFT_DIR)/libft.a

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBFT)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: all

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

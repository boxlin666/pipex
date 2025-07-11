NAME       := pipex
BONUS_NAME := pipex_bonus

CC         := cc
CFLAGS     := -Wall -Wextra -Werror -g
INCLUDES   := -I include -I libft

SRC_DIR    := src
BONUS_DIR  := bonus

SRCS       := $(addprefix $(SRC_DIR)/, main.c exec_cmd.c pipex.c error_exit.c)
OBJS       := $(SRCS:.c=.o)

BONUS_SRCS := $(addprefix $(BONUS_DIR)/, main_bonus.c run_pipex_bonus.c exec_cmd_bonus.c init_pipex_bonus.c init_here_doc_bonus.c error_exit_bonus.c util_pipex_bonus.c)
BONUS_OBJS := $(BONUS_SRCS:.c=.o)

LIBFT_DIR  := libft
LIBFT      := $(LIBFT_DIR)/libft.a

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBFT)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(BONUS_OBJS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

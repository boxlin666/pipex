# **************************************************************************** #
#                                Project Setup                                 #
# **************************************************************************** #

NAME        := pipex
BONUS_NAME  := pipex_bonus

CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -g
INCLUDES    := -I include -I libft

SRC_DIR     := src
BONUS_DIR   := bonus

SRCS        := $(addprefix $(SRC_DIR)/, main.c exec_cmd.c pipex.c utils.c)
BONUS_SRCS  := $(addprefix $(BONUS_DIR)/, main_bonus.c run_pipex_bonus.c exec_cmd_bonus.c init_pipex_bonus.c init_here_doc_bonus.c error_exit_bonus.c utils_bonus.c)

OBJS        := $(SRCS:.c=.o)
BONUS_OBJS  := $(BONUS_SRCS:.c=.o)

DEPS        := $(OBJS:.o=.d)
BONUS_DEPS  := $(BONUS_OBJS:.o=.d)

# **************************************************************************** #
#                                Libft Setup                                   #
# **************************************************************************** #

LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBFT)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(BONUS_OBJS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

-include $(DEPS)
-include $(BONUS_DEPS)

# **************************************************************************** #
#                                   Cleaning                                   #
# **************************************************************************** #

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(BONUS_OBJS) $(DEPS) $(BONUS_DEPS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

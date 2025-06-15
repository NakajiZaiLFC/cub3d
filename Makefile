# Program name
NAME = cub3D

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
DEBUG_CFLAGS = -Wall -Wextra -Werror -g -DDEBUG -fsanitize=address
INCLUDES = -I./include -I./minilibx

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Source files
SRC_FILES = main.c \
			init.c \
			events.c \
			render.c \
			cleanup.c \
			error.c \
			parser.c \
			map_parser.c \
			map_validation.c \
			raycasting.c \
			texture.c \
			minimap.c \
			utils.c \
			file_utils.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# MiniLibX
MLX_DIR = minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a

# OS detection
UNAME_S := $(shell uname -s)

# Linking flags
ifeq ($(UNAME_S),Linux)
	LDFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm
else ifeq ($(UNAME_S),Darwin)
	LDFLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit -lm
endif

# URLs for MinilibX
LINUX_URL = https://cdn.intra.42.fr/document/document/35303/minilibx-linux.tgz
DARWIN_URL = https://cdn.intra.42.fr/document/document/35302/minilibx_macos_metal.tgz

LINUX_FILE = minilibx-linux.tgz
DARWIN_FILE = minilibx_macos_metal.tgz

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

# Rules
all: $(NAME)

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: LDFLAGS += -fsanitize=address
debug: $(NAME)

$(NAME): $(MLX_LIB) $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX_LIB): setup-minilibx
	@echo "$(YELLOW)Building MinilibX...$(RESET)"
	@cd $(MLX_DIR) && make

setup-minilibx:
ifeq ($(UNAME_S),Linux)
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "$(YELLOW)Linux環境を検出しました。MinilibX for Linuxをダウンロードします...$(RESET)"; \
		curl -o $(LINUX_FILE) $(LINUX_URL); \
		tar -xzf $(LINUX_FILE); \
		if [ -d minilibx-linux ]; then \
			mv minilibx-linux minilibx; \
		elif [ -d mlx ]; then \
			mv mlx minilibx; \
		else \
			echo "$(RED)エラー: 解凍されたディレクトリが見つかりません$(RESET)"; \
			exit 1; \
		fi; \
		rm -f $(LINUX_FILE); \
	fi
else ifeq ($(UNAME_S),Darwin)
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "$(YELLOW)Mac環境を検出しました。MinilibX for macOSをダウンロードします...$(RESET)"; \
		curl -o $(DARWIN_FILE) $(DARWIN_URL); \
		tar -xzf $(DARWIN_FILE); \
		if [ -d minilibx_macos ]; then \
			mv minilibx_macos minilibx; \
		elif [ -d mlx ]; then \
			mv mlx minilibx; \
		else \
			echo "$(RED)エラー: 解凍されたディレクトリが見つかりません$(RESET)"; \
			exit 1; \
		fi; \
		rm -f $(DARWIN_FILE); \
	fi
else
	@echo "$(RED)エラー: サポートされていないOS $(UNAME_S)$(RESET)"
	@exit 1
endif

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@if [ -d "$(MLX_DIR)" ]; then \
		cd $(MLX_DIR) && make clean; \
	fi

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@rm -f $(NAME)

re: fclean all

help:
	@echo "$(BLUE)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)        - Build the project"
	@echo "  $(GREEN)debug$(RESET)      - Build with debug flags and memory sanitizer"
	@echo "  $(GREEN)clean$(RESET)      - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)     - Remove object files and executable"
	@echo "  $(GREEN)re$(RESET)         - Rebuild the project"
	@echo "  $(GREEN)help$(RESET)       - Show this help message"
	@echo "  $(GREEN)setup-minilibx$(RESET) - Setup MinilibX library"

.PHONY: all debug clean fclean re help setup-minilibx
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/15 16:45:41 by rapohlen          #+#    #+#              #
#    Updated: 2026/03/20 17:32:54 by rapohlen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files
CFILES		= main.c \
			  build_env.c \
			  setup_signal_handlers.c \
			  cleanup.c \
			  parse.c \
			  parse_tokenize.c \
			  parse_tokenize_fill.c \
			  parse_tokenize_len.c \
			  parse_tokenize_util.c \
			  parse_organize.c \
			  parse_organize_fill.c \
			  execute.c \
			  print_error.c \
			  util.c \
			  util_env.c \
			  cd.c \
			  pwd.c \
			  echo.c
SRCDIR		= src
SRC			= $(addprefix $(SRCDIR)/, $(CFILES))

# Header directories
INCDIR		= inc \
			  libft/inc

# Build directory
BUILDDIR	= .build

# Object and dependency files
OBJ			= $(SRC:%.c=$(BUILDDIR)/%.o)
DEP			= $(OBJ:.o=.d)

# Output binary
NAME		= minishell

# Libraries
LIB			= $(LIBFT)
LINK		= -lreadline
LIBFT		= libft/libft.a
LIBFT_REPO	= https://github.com/m0d1nst4ll3r/43_mylibft

# Compiler settings
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP $(addprefix -I,$(INCDIR))

# Make settings
MAKEFLAGS	+= --no-print-directory
MAKEFLAGS	:= $(MAKEFLAGS) $(if $(filter -j%,$(MAKEFLAGS)),,-j)

# Default rule
all:		$(NAME)

# Link
$(NAME):	$(OBJ) $(LIB)
			$(CC) $(CFLAGS) $^ $(LINK) -o $@

# Build libraries
$(LIBFT):
			if [ ! -d $(dir $(LIBFT)) ]; then git clone $(LIBFT_REPO) $(dir $(LIBFT)); fi
			$(MAKE) -C $(@D)

# Compile source -> object (auto-create directories)
$(BUILDDIR)/%.o: %.c | $(LIB)
			@mkdir -p $(@D)
			$(CC) $(CFLAGS) -c -o $@ $<

# Valgrind memory test
valgrind:	$(NAME)
			valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp --gen-suppressions=all ./$(NAME)

# Cleanup
clean:
			@for f in $(dir $(LIB)); do $(MAKE) -C $$f clean; done
			rm -rf $(BUILDDIR)

fclean:
			@for f in $(dir $(LIB)); do $(MAKE) -C $$f fclean; done
			rm -rf $(NAME) $(BUILDDIR)

re:
			$(MAKE) fclean
			$(MAKE) all

# Include dependency files
-include $(DEP)

.PHONY: clean fclean re

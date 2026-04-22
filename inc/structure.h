/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:00:56 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/22 12:41:02 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

// ================================ PARSER LIST ================================
typedef enum e_parse_state
{
	STATE_QUOTE,
	STATE_DQUOTE,
	STATE_NONE,
	STATE_BEGIN,
	STATE_PIPE,
	STATE_REDIR,
	STATE_WORD
}	t_parse_state;

typedef enum e_token_type
{
	TOKEN_LESSER,
	TOKEN_GREATER,
	TOKEN_LESSERLESSER,
	TOKEN_GREATERGREATER,
	TOKEN_PIPE,
	TOKEN_WORD,
	TOKEN_EMPTY
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*word;
	struct s_token	*next;
}	t_token;

// =============================== COMMAND LIST ================================
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}	t_redir_type;

// Chained list for simplicity
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

// Removing name from the struct since argv[0] already has it
// Changing the way redirects work, since we can have several per command
// Making this into a chained list for simplicity
typedef struct s_command
{
	char				**argv;
	t_redir				*redir;
	int					heredoc_fd;
	struct s_command	*next;
}	t_command;

// ================================= EXECUTOR ==================================
typedef struct s_exec
{
	int			**pipe_fd;
	pid_t		last_pid;
	t_command	*cmd;
	int			nb_cmd;
}	t_exec;

// ================================ MAIN STRUCT ================================
typedef struct s_minishell
{
	char		**env;
	char		*line;
	t_command	*cmd_list;
	t_exec		exec;
	int			last_return;
	bool		forked;
}	t_minishell;

#endif

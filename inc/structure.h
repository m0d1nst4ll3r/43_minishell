/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:00:56 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 12:59:24 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

// ================================ PARSER LIST ================================
typedef enum e_token_type
{
	TOKEN_LESSER,
	TOKEN_GREATER,
	TOKEN_LESSERLESSER,
	TOKEN_GREATERGREATER,
	TOKEN_PIPE,
	TOKEN_WORD
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*word; // If token is not WORD, this is NULL
	struct s_token	*next;
}	t_token;

// =============================== COMMAND LIST ================================
typedef enum e_redir_type
{
	REDIR_IN, // <
	REDIR_OUT, // >
	REDIT_APPEND, // >>
	REDIR_HEREDOC // <<
}	t_redir_type;

// Chained list for simplicity
typedef struct s_redir
{
	t_redir_type	type;
	char			*file; // Also works for HEREDOC limiter
	struct s_redir	*next;
}	t_redir;

// Removing name from the struct since argv[0] already has it
// Changing the way redirects work, since we can have several per command
// Making this into a chained list for simplicity
typedef struct s_command
{
	char				**argv;
	t_redir				*redir;
	struct s_command	*next;
}	t_command;

// ================================ MAIN STRUCT ================================
typedef struct s_minishell
{
	char		**env;
	char		*line;
	t_command	*cmd_list;
	int			last_return;
}	t_minishell;

#endif

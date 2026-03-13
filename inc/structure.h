/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:00:56 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 19:11:15 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

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
typedef struct s_command_list
{
	char					**argv;
	t_redir					*redir;
	struct s_command_list	*next;
}	t_command_list;

typedef struct s_minishell
{
	char			**env;
	char			*line;
	t_command_list	*cmd_list;
	int				last_return;
}	t_minishell;

#endif

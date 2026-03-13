/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:00:56 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 18:14:24 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

typedef struct s_command
{
	char	*name;
	char	**args;
}	t_command;

typedef struct s_command_table
{
	int			num_cmd;
	t_command	*array;
	char		*infile;
	char		*outfile;
}	t_command_table;

typedef struct s_minishell
{
	char			**env;
	char			*line;
	t_command_table	cmd_table;
	int				last_return;
}	t_minishell;

#endif

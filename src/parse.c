/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 17:56:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command_table	parse(char *line)
{
	t_command_table	cmd_table;

	(void)line;
	cmd_table.num_cmd = 0;
	cmd_table.array = NULL;
	cmd_table.infile = NULL;
	cmd_table.outfile = NULL;
	return (cmd_table);
}

/*	Parser
 *
 * Lines follow
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/

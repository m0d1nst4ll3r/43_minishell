/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:33:14 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 15:40:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_line(char *line, t_command_table cmd_table)
{
	int	i;
	int	j;

	free(line);
	free(cmd_table.infile);
	free(cmd_table.outfile);
	i = 0;
	while (i < cmd_table.num_cmd)
	{
		free(cmd_table.array[i].name);
		j = 0;
		while (cmd_table.array[i].args[j])
		{
			free(cmd_table.array[i].args[j]);
			j++;
		}
		free(cmd_table.array[i].args);
		i++;
	}
}

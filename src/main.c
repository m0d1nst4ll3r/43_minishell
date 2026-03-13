/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:05:03 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/13 15:57:39 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Program steps:
// 1. Build own env (copy envp)
// 2. Setup signal handlers
// 3. while 1 readline
//		a. Readline call
//		b. Add history
//		c. Build command table (parser)
//		d. Execute command table (executor)
int	main(int ac, char **av, char **ep)
{
	t_minishell	data;
	(void)ac;
	(void)av;
	data.env = build_env(ep);
	setup_signal_handlers();
	while (1)
	{
		data.line = readline(PROMPT);
		if (!data.line)
			break ;
		add_history(data.line);
		data.cmd_table = parse(data.line);
		data.last_return = execute(data.cmd_table);
		cleanup_line(data.line, data.cmd_table);
	}
	cleanup_env(data.env);
	rl_clear_history();
	return (0);
}

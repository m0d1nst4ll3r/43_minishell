/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:05:03 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/21 14:15:20 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	event_hook(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		rl_replace_line("", 1);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	return (0);
}

static void	shell_loop(t_minishell *data)
{
	while (1)
	{
		g_signal = 0;
		if (set_sigint())
			error_out(data, ERR_SIGNAL);
		data->line = readline(PROMPT);
		if (!data->line)
			builtin_exit(0, NULL, data);
		add_history(data->line);
		data->cmd_list = parse(data);
		if (data->cmd_list)
			data->last_return = execute(data);
		free(data->line);
		cleanup_cmd_list(data->cmd_list, 1);
		data->cmd_list = NULL;
	}
}

// Program steps:
// 1. Build own env (copy envp)
// 2. Setup signal handlers
// 3. while 1 readline
//		a. Readline call
//		b. Add history
//		c. Build command table (parser)
//		d. Execute command table (executor)
//		e. Free line & command table
int	main(int ac, char **av, char **ep)
{
	t_minishell	data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(data));
	if (set_sigquit())
		error_out(&data, ERR_SIGNAL);
	data.env = build_env(&data, ep);
	data.last_return = 0;
	data.forked = false;
	rl_event_hook = event_hook;
	shell_loop(&data);
}

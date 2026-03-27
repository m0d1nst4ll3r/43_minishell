/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:05:03 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/27 17:12:06 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_prog(char **env)
{
	cleanup_env(env);
	rl_clear_history();
}

static void	error_stop(char **env, char *err_str)
{
	print_error(err_str);
	cleanup_prog(env);
	exit(1);
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
	if (setup_signal_handlers()) // Memo: restore defaults after fork()
		error_stop(data.env, ERR_SIGNAL);
	data.env = build_env(ep);
	data.last_return = 0;
	rl_event_hook = event_hook;
	while (1)
	{
		if (g_signal == SIGINT)
		{
			write(1, "\n", 1);
			g_signal = 0;
		}
		data.line = readline(PROMPT);
		if (!data.line)
			break ;
		add_history(data.line);
		printf("You typed: %s\n", data.line);
		data.cmd_list = parse(&data);
		data.last_return = execute(&data); // Memo: Care about passing NULL env to execve in case of failed malloc
		free(data.line);
		cleanup_cmd_list(data.cmd_list, 1);
	}
	cleanup_prog(data.env);
	return (0);
}

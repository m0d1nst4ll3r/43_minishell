/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:05:03 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/20 17:52:31 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_prog(char **env)
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
	int 		retval;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(data));
	if (set_sigquit())
		error_stop(data.env, ERR_SIGNAL);
	data.env = build_env(ep);
	data.last_return = 0;
	data.forked = false;
	rl_event_hook = event_hook;
	while (1)
	{
		if (set_sigint())
		{
			; // error_out()
		}
		data.line = readline(PROMPT);
		if (unset_sigint())
		{
			; // error_out()
		}
		if (!data.line)
		{
			printf("exit\n");
			break ;
		}
		add_history(data.line);
		data.cmd_list = parse(&data);
		if (data.cmd_list)
			data.last_return = execute(&data);
		free(data.line);
		cleanup_cmd_list(data.cmd_list, 1);
	}
	retval = data.last_return;
	cleanup_prog(data.env);
	return (retval);
}

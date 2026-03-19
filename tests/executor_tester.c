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

int	main(int ac, char **av, char **ep)
{
	t_minishell	data;

    (void)ac;
    (void)av;
    if (setup_signal_handlers())
		error_stop(data.env, ERR_SIGNAL);
	data.env = build_env(ep);
    data.last_return = 0;
	while (1)
	{
		data.line = readline(PROMPT);
		if (!data.line)
			break ;
		add_history(data.line);
		data.cmd_list = parse(&data);
		data.last_return = execute(&data);
		free(data.line);
		cleanup_cmd_list(data.cmd_list, 1);
	}
	cleanup_prog(data.env);
	return (0);
}
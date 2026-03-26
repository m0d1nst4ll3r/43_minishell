/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:03:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/26 12:39:25 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

//
//	Shell setup
//
char			**build_env(char **envp);
int				setup_signal_handlers(void);
int				reset_signal_handlers(void);

//
//	Main loop
//
t_command		*parse(t_minishell *data);
int				execute(t_minishell *data);

//
//	Parser
//
// Tokenizer
t_token			*tokenize(t_minishell *d);
int				get_word_len(t_minishell *d, size_t i);
// Tokenizer Util
t_token_type	get_token_type(char *line);
int				is_end_of_word(char c, t_parse_state state);
void			update_state(char c, t_parse_state *state, size_t *i);
// Organizer
t_command		*organize(t_token *token_list);
int				fill_cmd(t_token **token_list, t_command *cmd);
int				create_new_cmd(t_command **cmd_list, size_t argv_count,
					t_command **last);

//
//	Executor
//
// Heredoc
char			*expand_line(char *line, t_minishell *d);

//
//	Built-in
//
int				builtin_cd(int ac, char **av, char **ep);
int				builtin_echo(int ac, char **av, char **ep);
int				builtin_pwd(int ac, char **av, char **ep);
int				builtin_env(int ac, char **av, char **ep);
int				builtin_export(int ac, char **av, char ***ep);
int				builtin_unset(int ac, char **av, char ***ep);

//
//	Util
//
void			*ft_malloc(size_t size);
int				is_envar_char(char c);
int				is_valid_envar_syntax(char *line);
char			*get_env(char *key, char **env);
size_t			get_env_size(char **env);
void			get_expanded_envar_len(char *line, t_minishell *d, size_t *i,
					size_t *len);
size_t			write_expanded_envar(char *line, t_minishell *d, size_t *len,
					char *word);
// Error
void			print_error(char *err_str);
void			print_error_builtin(char *name, char *err_str);
void			print_error_builtin_file(char *name, char *file, char *err_str);
// Cleanup
void			cleanup_env(char **env);
void			cleanup_token_list(t_token *token_list, int clean_contents);
void			cleanup_cmd_list(t_command *cmd_list, int clean_contents);

#endif

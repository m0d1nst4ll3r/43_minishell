/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:03:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:04:16 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

//
//	Shell setup
//
char			**build_env(char **envp);
int				setup_signal_handlers(void);

//
//	Main loop
//
t_command		*parse(t_minishell *data);
int				execute(t_minishell *data);

//
//	Parser
//
// Expander
char			*get_expanded_line(t_minishell *d);
void			fill_expanded_line(t_minishell *d, char *exp_line);
// Tokenizer
t_token			*tokenize(char *line);
void			fill_word(char *line, size_t *i, char *word);
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

//
//	Built-in
//
unsigned char	cd(int ac, char **av, char **ep);

//
//	Util
//
void			*ft_malloc(size_t size);
int				is_envar_char(char c);
size_t			get_uchar_len(unsigned char u);
char			*get_env(char *key, char **env);
char			*get_env_n(char *key, char **env, size_t n);
// Error
void			print_error(char *err_str);
void			print_error_builtin(char *name, char *err_str);
// Cleanup
void			cleanup_env(char **env);
void			cleanup_token_list(t_token *token_list, int clean_contents);
void			cleanup_cmd_list(t_command *cmd_list, int clean_contents);

#endif

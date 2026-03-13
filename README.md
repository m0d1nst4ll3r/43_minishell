# Minishell

[Subject](https://cdn.intra.42.fr/pdf/pdf/199852/en.subject.pdf)

### Goals

1. Write a loop logic with readline which will
	a. Display a prompt when waiting for a new command
	b. Have a working history
	c. Display a new prompt on a new line with Ctrl-C (SIGINT)
	d. Exit the shell with Ctrl-D (only if line empty?)
	e. Do nothing on Ctrl-\ (SIGQUIT)
2. Write parser logic which will read and execute the user's commands
	a. Anything in "" counts as one word but $ env vars inside are still expanded
	b. Anything in '' counts as one word AND $ env vars are not expanded
	c. Pipes redirect previous cmd stdout to next cmd stdin
	d. < redirects 1st cmd stdin to a file
	e. << redirects 1st cmd stdin to a heredoc temp file with a limiter keyword
	f. > redirects last cmd stdout to a file (O_TRUNC)
	g. >> redirects last cmd stdout to a file (O_APPEND)
	h. $ env vars are expanded to their env value
	i. $? expands to last cmd's return value
3. Write built-in commands
	a. echo with -n
	b. cd with relative/absolute path
	c. pwd
	d. export
	e. unset
	f. env
	g. exit

1. and 3. are relatively simple. 2. is not that hard. No need for a lexer, tokenizer or anything. Just need a simple logic to decompose a line into a command structure (parser), and then execute it (executor).

### 1 - Basic loop & signals

```c
while (1)
{
	line = readline(PROMPT);
	if (!line)
		exit_shell();
	add_history(line);
	command_table = parse(line);
	last_return = execute(command_table);
}
```

Simple enough.

For signals, SIGQUIT should be ignored. SIGINT should call a new readline on a new line (unsure how to do this yet).

There should be no need for any global variables (?).

### 2 - Parser/Executor

The simplest is to decompose the line into a series of commands with their name (the first word that appears), and everything that follows. The structure should also have a stdin and stdout, either NULL or a filename.

```c
typedef struct s_command_table
{
	t_command	*commands; // cmd0 stdout will go into cmd1 stdin, etc...
	char		*stdin; // NULL if no redirection
	char		*stdout;
}	t_command_table;
```

Each command looks like:

```c
typedef struct s_command
{
	char	*name; // Un-transformed 1st word (only $ is expanded), executor will handle rest
	char	**argv; // Un-transformed 2nd, 3rd, etc... words (only $ is expanded)
}	t_command;
```

The *parser* makes these two structs. The executor will then take them and handle them, returning the return value which will be written in `last_return` ($? expands to that, it is 0 on startup).

This is essentially pipex, but with a better parser that handles $ " '.

### 3 - Built-in commands

Built-ins do not try to read from stdin (stdin redirections will do nothing). Invalid redirections still prevent cmd0 from executing, even if it is a built-in.

`cd` requires parsing argv[1], `cd ../../././home/rapohlen/../rapohlen/././Downloads/` needs to work.

`echo` is a dumb printf in a loop with an optional \n at the end (depending on -n). -n can only be in argv[1].

`pwd` is a printf of getcwd's return. Can eventually make size potentially infinite by looking at ERANGE error return and using malloc + doubling size for next try, with a reasonable limit (like 32MB).

`export` haven't looked at it yet, need a func to add to env, or build our own envp `char **`.

`unset` same as above.

`env` dumb printf in a while.

`exit` just cleanup + leave.

### Program structure

```c
typedef struct s_minishell
{
	char			*line; // Return from readline
	t_command_table	command_table; // Created by parser, read by executor
	unsigned char	last_return; // 0 to 255
	char			**env; // Custom environment, needed for export, unset, env
}	t_minishell;
```

Not much else? Seems like a simple project.

### Details

This is where the fun begins

1. `/../../../../../../../../../../../../` is `/`, meaning if `..` can't go higher, it is ignored and doesn't cause any errors
	- This is an important detail for `cd`
2. `ls > out > out2 > out3 > out4 > out5 > out6 > lol > this > project > is > gonna > be > fun` is valid input and *ALL* the files will be created, although only the file that appears *last* will contain the output
	- Similarly, `> out ls` works and `> out > out2 > out3 ls > out4` works (only out4 will contain the output)
	- `ls > lsout | grep l > grepout | wc -l > wcout` is perfectly valid, each command's stdout is redirected to its respective file, but since stdout is already redirected to a file, it is not redirected to the pipe, and therefore both grep and wc will have no input
3. `/../../../../../../home/rapohlen/./././../../usr/bin/cat blahblahblah` prints `/../../../../../../home/rapohlen/./././../../usr/bin/cat: blahblahblah: No such file or directory`, so it is clear that argv[0] contains whatever string was sent to bash rather than a curated `cat` version
4. `2>&1` etc... is out-of-scope and should not be handled (only basic > < >> <<)
	- This will cause differences with bash but... can't recode bash entirely
5. `ls > out | wc < out` causes a display of `0 0 0` because wc reads out AFTER it was opened with `O_TRUNC` but BEFORE ls writes anything to it. Theoretically anything can happen, but in reality wc is always faster than ls.

# Minishell

[Subject](https://cdn.intra.42.fr/pdf/pdf/199852/en.subject.pdf)

### Goals

1. Write a loop logic with readline which will
	1. Display a prompt when waiting for a new command
	2. Have a working history
	3. Display a new prompt on a new line with Ctrl-C (SIGINT)
	4. Exit the shell with Ctrl-D (only if line empty?)
	5. Do nothing on Ctrl-\ (SIGQUIT)
2. Write parser logic which will read and execute the user's commands
	1. Anything in "" counts as one word but $ env vars inside are still expanded
	2. Anything in '' counts as one word AND $ env vars are not expanded
	3. Pipes redirect previous cmd stdout to next cmd stdin
	4. < redirects 1st cmd stdin to a file
	5. << redirects 1st cmd stdin to a heredoc temp file with a limiter keyword
	6. > redirects last cmd stdout to a file (O_TRUNC)
	7. >> redirects last cmd stdout to a file (O_APPEND)
	8. $ env vars are expanded to their env value
	9. $? expands to last cmd's return value
3. Write built-in commands
	1. echo with -n
	2. cd with relative/absolute path
	3. pwd
	4. export
	5. unset
	6. env
	7. exit

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

Parser fills a chained list containing commands and their respective redirections, in the order in which they appear in pipes. This is sent to the executor.

No need for tokens imo.

This is 80% of the work.

### 3 - Built-in commands

Built-ins do not try to read from stdin (stdin redirections will do nothing). Invalid redirections still prevent cmd0 from executing, even if it is a built-in.

`cd` requires parsing argv[1], `cd ../../././home/rapohlen/../rapohlen/././Downloads/` needs to work.

`echo` is a dumb printf in a loop with an optional \n at the end (depending on -n). -n can only be in argv[1].

`pwd` is a printf of getcwd's return. Can eventually make size potentially infinite by looking at ERANGE error return and using malloc + doubling size for next try, with a reasonable limit (like 32MB).

`export` needs to add to our custom `char **env` array, therefore re-malloc'ing and copying over everything - this is tedious and inefficient but it is better than a chained list because you just pass it as-is to execve

`unset` same as above.

`env` dumb printf in a while.

`exit` just cleanup + leave.

### Details

This is where the fun begins

1. `/../../../../../../../../../../../../` is `/`, meaning if `..` can't go higher, it is ignored and doesn't cause any errors
	- This is an important detail for `cd`
	- Update: turns out chdir handles this itself, so we don't have to care or build a crazy parser for this
2. `ls > out > out2 > out3 > out4 > out5 > out6 > lol > this > project > is > gonna > be > fun` is valid input and *ALL* the files will be created, although only the file that appears *last* will contain the output
	- Similarly, `> out ls` works and `> out > out2 > out3 ls > out4` works (only out4 will contain the output)
	- `ls > lsout | grep l > grepout | wc -l > wcout` is perfectly valid, each command's stdout is redirected to its respective file, but since stdout is already redirected to a file, it is not redirected to the pipe, and therefore both grep and wc will have no input
3. `/../../../../../../home/rapohlen/./././../../usr/bin/cat blahblahblah` prints `/../../../../../../home/rapohlen/./././../../usr/bin/cat: blahblahblah: No such file or directory`, so it is clear that argv[0] contains whatever string was sent to bash rather than a curated `cat` version
4. `2>&1` etc... is out-of-scope and should not be handled (only basic > < >> <<)
	- This will cause differences with bash but... can't recode bash entirely
5. `ls > out | wc < out` causes a display of `0 0 0` because wc reads out AFTER it was opened with `O_TRUNC` but BEFORE ls writes anything to it. Theoretically anything can happen, but in reality wc is always faster than ls.
6. Just spaces (and tabs and newlines, though we don't handle newlines anyway) does nothing - shouldn't segfault
	- In our logic our `cmd_list` will just be NULL if we only encountered whitespaces
7. `export test=lol | echo $test` prints nothing but a newline, because export runs in its own subshell (fork), so the env modifications cannot be reflected in the next subshell that runs echo
	- Similarly, `exit | echo test` does not exit, the exit would run in the sub-shell
8. execve parses the program path by itself - you can send `/../../../../usr/././././bin/../bin/ls`
9. Variables can be environment variables (displayed with `env`, passed to child processes), but they can also be *shell* variables, visible in the shell but not with `env` and not passed to children
	- `export` is the way to make a shell variable into an environment variable
	- Just typing `FOO=bar` creates a *shell* variable, invisible in `env`
	- bash has *built-in* shell variables, such as **IFS** - that one contains ' \t\n', which is the list of word-separators used after expansions (not by the lexical parsing of the line)
	- You can *modify* IFS to *add different word-separators*. Obviously this is cosmic-level absurd complexity and we should not handle this.

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFER_SIZE	64

static void	print_stats(char *file, struct stat stats)
{
	printf("Stats of %s:\n", file);
	printf("\tID of device containing file: %ld\n", stats.st_dev);
	printf("\tInode number: %ld\n", stats.st_ino);
	printf("\tFile type and mode: %d\n", stats.st_mode);
	printf("\tNumber of hard links: %ld\n", stats.st_nlink);
	printf("\tUser ID of owner: %d\n", stats.st_uid);
	printf("\tGroup ID of owner: %d\n", stats.st_gid);
	printf("\tDevice ID (if special file): %ld\n", stats.st_rdev);
	printf("\tTotal size, in bytes: %ld\n", stats.st_size);
	printf("\tBlock size for filesystem I/O: %ld\n", stats.st_blksize);
	printf("\tNumber of 512 B blocks allocated: %ld\n", stats.st_blocks);
}

static void	read_dir_contents(char *dirname)
{
	DIR				*dirp;
	struct dirent	*dirent;

	dirp = opendir(dirname);
	if (!dirp)
		printf("%s: %s\n", dirname, strerror(errno));
	else
	{
		while (1)
		{
			dirent = readdir(dirp);
			if (!dirent)
			{
				if (errno)
					printf("%s: %s\n", dirname, strerror(errno));
				break ;
			}
			printf("%s\n", dirent->d_name);
		}
		closedir(dirp);
	}
}

int	main(int ac, char **av)
{
	struct stat	stats;
	char		buffer[BUFFER_SIZE];
	char		*line;

	(void)ac;
	while (1)
	{
		line = readline(av[1]);
		if (!line || !strcmp(line, "exit"))
		{
			free(line);
			return (0);
		}
		else if (!strcmp(line, "clear"))
		{
			printf("History cleared\n");
			rl_clear_history();
			free(line);
		}
		else if (!strcmp(line, "pwd"))
		{
			add_history(line);
			if (getcwd(buffer, BUFFER_SIZE))
				printf("%s\n", buffer);
			free(line);
		}
		else if (!strncmp(line, "cd ", 3) && line[3])
		{
			add_history(line);
			if (chdir(line + 3) == -1)
				printf("%s: %s\n", line + 3, strerror(errno));
			free(line);
		}
		else if (!strncmp(line, "stat ", 5) && line[5])
		{
			add_history(line);
			if (stat(line + 5, &stats) == -1)
				printf("%s: %s\n", line + 5, strerror(errno));
			else
				print_stats(line + 5, stats);
			free(line);
		}
		else if (!strncmp(line, "lstat ", 6) && line[6])
		{
			add_history(line);
			if (lstat(line + 6, &stats) == -1)
				printf("%s: %s\n", line + 6, strerror(errno));
			else
				print_stats(line + 6, stats);
			free(line);
		}
		else if (!strcmp(line, "ls") || !strncmp(line, "ls ", 3))
		{
			add_history(line);
			if (!line[2] || !line[3])
				read_dir_contents(".");
			else
				read_dir_contents(line + 3);
			free(line);
		}
		else
		{
			add_history(line);
			printf("You typed: %s\n", line);
			free(line);
		}
	}
	return (0);
}

#include "microshell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	puterr(char *str)
{
	write(2, str, ft_strlen(str));
}

int	cd_cmd(char *argv[], int i)
{
	if (i != 2)
		return (puterr("error: cd: bad arguments\n"), 1);
	if (chdir(argv[1]) == -1)
		return (puterr("error: cd: cannot change directory to "), \
		puterr(argv[1]), puterr("\n"), 1);
	return (0);
}

int	redirect_pipe(int in_out[], int to_pipe, int in_or_out)
{
	if (to_pipe == 0)
		return (0);
	if (dup2(in_out[in_or_out], in_or_out) == -1 || \
	close(in_out[0]) == -1 || close(in_out[1]) == -1)
		return (puterr("error: fatal\n"), exit (1), 1);
	return (0);
}

int	gen_exec(char *argv[], int i, char *envp[])
{
	int		in_out[2];
	pid_t	pid;
	int		to_pipe;
	int		status;

	to_pipe = 0;
	if (argv[i] != NULL && strcmp(argv[i], "|") == 0)
		to_pipe = 1;
	if (to_pipe != 1 && strcmp("cd", argv[0]) == 0)
		return (cd_cmd(argv, i));
	if (to_pipe == 1 && pipe(in_out) == -1)
		return (puterr("error: fatal\n"), exit (1), 1);
	pid = fork();
	if (pid == -1)
		return (puterr("error: fatal\n"), exit (1), 1);
	if (pid == 0)
	{
		argv[i] = NULL;
		redirect_pipe(in_out, to_pipe, STDOUT_FILENO);
		if (strcmp(argv[0], "cd") == 0)
			exit(cd_cmd(argv, i));
		execve(argv[0], argv, envp);
		return (puterr("error: cannot execute "), \
		puterr(argv[0]), puterr("\n"), exit(1), 1);
	}
	waitpid(pid, &status, 0);
	redirect_pipe(in_out, to_pipe, STDIN_FILENO);
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	status;

	if (argc == 1)
		return (0);
	i = 0;
	while (argv[i] != NULL)
	{
		argv += i + 1;
		i = 0;
		while (argv[i] != NULL && strcmp(argv[i], "|") != 0 && \
								strcmp(argv[i], ";") != 0)
			i++;
		if (i)
			status = gen_exec(argv, i, envp);
	}
	return (status);
}

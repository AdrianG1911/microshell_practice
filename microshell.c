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

int	gen_exec(char *argv[], int i, char *envp[])
{
	int		pipe[2];
	pid_t	pid;
	
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	status;

	if (argc == 1)
		return (0);
	i = 1;
	while (i < argc)
	{
		argv += i;
		i = 0;
		while (argv[i] != NULL && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		status = gen_exec(argv, i, envp);
	}
}

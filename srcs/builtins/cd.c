#include "minishell.h"

static void	ft_pwd_error(t_config *config)
{
	ft_error(SHNAME, NULL, PATH_MAX_ERR, CR_ERROR | SERROR);
	config->pwd_subrep = "???";
	config->prompt_len = 9;
}

void		ft_update_pwd(t_config *config)
{
	char	buf[_POSIX_PATH_MAX];
	char	*pwd;

	ft_freegiveone((void**)&(config->pwd));
	if (!getcwd(buf, _POSIX_PATH_MAX))
		ft_pwd_error(config);
	else
	{
		if (!(pwd = ft_strtabfindstart(config->env, "PWD="))
			|| strcmp(buf, pwd + 4))
			ft_setenv("PWD", buf, config);
		if (!(pwd = ft_strdup(buf)))
			ft_pwd_error(config);
		else
		{
			config->pwd = pwd;
			if (!pwd[1])
				config->pwd_subrep = pwd;
			else
				config->pwd_subrep = ft_strrchr(pwd, '/') + 1;
			config->prompt_len = ft_strlen(config->pwd_subrep) + 6;
		}
	}
}

static void	ft_path_follow(char *path, t_config *config)
{
	if (path[1] && !ft_access_dir(path))
		;
	else if (!chdir(path))
		ft_update_pwd(config);
	else
		ft_error(SHNAME, "failed moving to directory", path, CR_ERROR);
	free(path);
}

void		ft_cd(char **argv, t_config *config)
{
	char	*path;
	int		i;

	path = NULL;
	if ((i = (ft_strcmp(argv[0], "cd") ? 0 : 1)) && ft_strtablen(argv) - i > 1
		&& ft_error(SHNAME, "cd", "too many arguments", CR_ERROR))
		return ;
	else if (!argv[i] && (path = ft_strtabfind(config->env, "HOME=")))
		path = ft_strdup(path + 5);
	else if (argv[i] && argv[i][0] == '/')
		path = ft_strdup(argv[i]);
	else if (argv[i] && argv[i][0] == '~'
			&& (path = ft_strtabfind(config->env, "HOME=")))
		path = ft_strslashjoin(path + 5,
				(ft_strlen(argv[i]) > 2 ? argv[i] + 2 : "."));
	else if (argv[i] && argv[i][0] == '-'
			&& (path = ft_strtabfind(config->env, "OLDPWD=")))
		path = ft_strslashjoin(path + 7,
				(ft_strlen(argv[i]) > 2 ? argv[i] + 2 : "."));
	else if (argv[i] && (path = config->pwd))
		path = ft_strslashjoin(path, argv[i]);
	if (path)
		ft_path_follow(path, config);
	else
		ft_error(SHNAME, "cd", "something is missing in env", CR_ERROR);
}

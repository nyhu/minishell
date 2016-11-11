
#include "minishell.h"

static void	ft_heredoc(t_list *begin, t_config *config)
{
	char		**kill;
	char		*tmp;

	config->shell_state = SCANNING_COMMAND;
	config->heredoc = 1;
	config->pwd_subrep = "heredoc> ";
	config->prompt_len = 9;
	tmp = ft_streamscan(config, ft_save_stream(NULL), 0);
	while (!tmp || ft_strcmp(tmp, ((char**)begin->next->data)[0]))
	{
		kill = (char**)begin->next->data;
		if (!(begin->next->data = ft_strtabadd((char**)begin->next->data,
			((tmp) ? tmp : ft_strnew(1)))))
		{
			ft_error(SHNAME, "heredoc", "malloc error", CR_ERROR);
			return ;
		}
		ft_freegiveone((void**)&kill);
		tmp = ft_streamscan(config, ft_save_stream(NULL), 0);
	}
	config->shell_state = RUNNING_COMMAND;
	ft_update_pwd(config);
	config->heredoc = 0;
}

static int	ft_decant(t_list *cmd, t_list *src, int i)
{
	char	**kill;

	if (!i && (!src || !((char**)src->data)[0] || !((char**)src->data)[0][0]))
	{
		if (src && src->next)
			ft_error(SHNAME, "parse error near", SNDATA, CR_ERROR);
		else
			ft_error(SHNAME, "parse error near", "newline", CR_ERROR);
		return (0);
	}
	if (!src)
		return (1);
	while (((char**)src->data)[++i])
	{
		kill = (char**)cmd->data;
		cmd->data = (void*)ft_strtabadd((char**)cmd->data,
				((char**)src->data)[i]);
		((char**)src->data)[i] = NULL;
		ft_freegiveone((void**)&kill);
	}
	return (1);
}

static int	ft_target_decant(t_list *begin, t_list *cmd)
{
	if (begin->data_size == HEREDOC || (begin->data_size == OP
				&& !ft_strchr(BDATA, '&') && (ft_strstr(BDATA, "<")
					|| ft_strstr(BDATA, ">"))))
	{
		if (!ft_decant(cmd, begin->next, 0))
			return (0);
	}
	else if ((begin->data_size == OP && ft_strchr(BDATA, '&')
				&& (ft_strstr(BDATA, "<") || ft_strstr(BDATA, ">"))))
	{
		if (!ft_decant(cmd, begin->next, -1))
			return (0);
	}
	return (1);
}

int			ft_herringbone(t_list *begin, t_config *config)
{
	t_list	*cmd;

	cmd = begin;
	while (begin)
	{
		if (!ft_target_decant(begin, cmd))
			return (0);
		else if (begin->data_size == PIPE || begin->data_size == OP)
		{
			if (BDATA[0] == '|' && (!begin->next || !begin->next->data
				|| (!begin->next->data_size && !((char**)begin->next->data)[0])))
				return (1 ^ ft_error(SHNAME, "parse error near",
							BDATA, CR_ERROR));
			cmd = begin->next;
		}
		else if (begin->data_size == SSHELL && !ft_herringbone(begin->data, config))
			return (0);
		if (begin->data_size == HEREDOC)
			ft_heredoc(begin, config);
		begin = begin->next;
	}
	return (1);
}

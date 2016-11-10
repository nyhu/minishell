#include "../includes/autocomp.h"
/*
** Obtient la taille de la liste COMP_SIZE_LIST.
*/
void		get_size_list(t_stream *stream)
{
	t_list	*list;
	size_t	i;

	if (COMP_BEGIN_LIST)
	{
		i = 1;
		list = COMP_BEGIN_LIST;
		while (list->next)
		{
			i++;
			list = list->next;
		}
	COMP_SIZE_LIST = i;
	}
}
/*
** Lancée sur chaque chaîne, permet d'obtenir le padding de la plus grande.
*/
void		get_pad(t_stream *stream, char *str)
{
	size_t	size;

	if ((size = ft_strlen(str)) > COMP_PAD)
		COMP_PAD = size;
}
/*
** Permet d'effacer la liste. Remet COMP_STATE à 0.
*/
void		ft_end_autocomp(t_stream *stream)
{
	size_t pos_buf;

	pos_buf = stream->pos;
	COMP_STATE = 0;
	ft_goend(stream);
	stream->tput = "cd";
	ft_tputs(stream);
	ft_gomatch(stream, pos_buf, ft_mvleft);
}
/*
** Check si la touche pressée interrompt l'autocomp.
** Si pas d'interruption retourne 1 sinon 0.
*/
int		ft_is_same_autocomp(t_stream *stream)
{
	static ssize_t	match[] = {LEF, RIG, UPP, DOW, CHT};
	int				i;
	int				is_same;

	i = -1;
	is_same = 0;
	while (match[++i])
		if (((ssize_t*)(stream->buf))[0] == match[i])
			is_same = (i == 4) ? 2 : 1;
	if (!is_same)
		ft_end_autocomp(stream);
	else if ((COMP_STATE == 1 || COMP_STATE == 0)  && is_same != 2)
		ft_end_autocomp(stream);
	else
		return (1);
	return (0);
}
/*
** Réinitialise la liste. Libère tous les éléments de la liste et COMP_BEGIN.
** Met tous les éléments statiques de la structure à 0.
*/
void	reset_autocomp(t_stream *stream)
{
	ft_lstdel(&(COMP_BEGIN_LIST), ft_list_free_data);
	ft_freegiveone((void**)&(COMP_BEGIN));
	bzero(&(stream->comp), sizeof(t_comp));

}


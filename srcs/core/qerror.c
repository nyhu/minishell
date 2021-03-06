/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qerror.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tboos <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/18 14:13:05 by tboos             #+#    #+#             */
/*   Updated: 2016/11/18 14:13:07 by tboos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_qerr(char *err)
{
	if (!ft_strcmp(err, PAR_ERR))
		return ("(");
	if (!ft_strcmp(err, BACK_ERR))
		return ("\\");
	return (")");
}

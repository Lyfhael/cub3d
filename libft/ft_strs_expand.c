/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strs_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:07:41 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/19 16:10:26 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strs_expand(char **origin, char *to_add)
{
	char	**result;
	int		len;
	int		i;

	i = -1;
	len = ft_strslen(origin);
	result = ft_calloc(len + 1 + 1, sizeof(char *));
	if (!result)
		return (NULL);
	while (++i < len)
	{
		result[i] = ft_strdup(origin[i]);
		if (!result[i])
			return (ft_strsfree(result), NULL);
	}
	result[i++] = ft_strdup(to_add);
	result[i] = 0;
	ft_strsfree(origin);
	free(to_add);
	return (result);
}

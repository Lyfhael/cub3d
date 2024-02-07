/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_endswith.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 08:10:50 by hateisse          #+#    #+#             */
/*   Updated: 2024/01/10 09:40:44 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

bool	ft_endswith(char *haystack, char *needle)
{
	size_t	hi;
	size_t	ni;

	if (!haystack || !needle)
		return (false);
	if (!*needle)
		return (true);
	if (!*haystack)
		return (false);
	hi = ft_strlen(haystack);
	ni = ft_strlen(needle);
	while (ni-- > 0 && hi-- > 0)
	{
		if (haystack[hi] != needle[ni])
			return (false);
		if (ni == 0)
			return (true);
	}
	return (false);
}

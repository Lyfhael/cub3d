/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_startswith.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 21:39:03 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/08 15:34:47 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_str_startswith(char *str, char *needle)
{
	if (!str || !needle || !*str)
		return (false);
	while (*str)
	{
		if (!*needle)
			return (true);
		else if (*str != *needle)
			return (false);
		str++;
		needle++;
	}
	return (true);
}

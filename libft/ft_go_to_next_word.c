/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_whitespaces.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 12:52:17 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/08 15:05:14 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_go_to_next_word(char *str)
{
	if (!str)
		return (str);
	while (!ft_is_whitespace(*str) && *str)
		str++;
	while (ft_is_whitespace(*str) && *str)
		str++;
	return (str);
}

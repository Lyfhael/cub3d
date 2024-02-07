/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup_next_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:02:01 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/01 14:11:33 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_dup_next_word(char *str)
{
	size_t	i;
	char	*word;

	if (!str)
		return (str);
	while (!ft_is_whitespace(str[i]) && str[i])
		i++;
	word = ft_calloc(i + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strncpy(word, str, i);
	return (str);
}

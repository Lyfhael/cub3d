/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:48:09 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/01 15:40:19 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <stdbool.h>
# include <unistd.h>
# include <sys/fcntl.h>

# define FAILURE 0
# define SUCCESS 1
# define UNDEFINED ((void*)0)

# define NORTH "NORTH_ID"
# define SOUTH "SOUTH_ID"
# define WEST "WEST_ID"
# define EAST "EAST_ID"
# define FLOOR "FLOOR_ID"
# define CEILING "CEILING_ID"

# define RGB_DELIM ','

typedef int		t_rgb;

typedef struct s_textures
{
	char		*no_wall;
	char		*so_wall;
	char		*we_wall;
	char		*ea_wall;
	t_rgb		*floor;
	t_rgb		*ceiling;
}				t_textures;

typedef struct s_c3d_map
{
	char		**map;
	t_textures	textures;
}				t_c3d_map;

enum e_conf_arg_type
{
	PRIMARY,
	MAP
};

enum e_conf_errors
{
	DUPLICATE_TEXTURE = -1,
	DUPLICATE_NO_TEXTURE = -2,
	DUPLICATE_SO_TEXTURE = -3,
	DUPLICATE_WE_TEXTURE = -4,
	DUPLICATE_EA_TEXTURE = -5,
	DUPLICATE_FLOOR_TEXTURE = -6,
	DUPLICATE_CEILING_TEXTURE = -7,
	UNKNOWN_PARAM = -8,
	RGB_OVERFLOW = -9,
	RGB_INVALID_DELIM = -10,
	RGB_INVALID_ENDING = -11,
	RGB_INVALID = -12,
	EMPTY_TEXTURE_PATH = -13
};

enum e_general_errors
{
	OPEN_FAILED,
	READ_FAILED,
	MALLOC_FAILED
};

enum e_parse_errors
{
	ARG_NO_INVALID,
	MAP_FILENAME_INVALID,
	INVALID_MAP
};

#endif
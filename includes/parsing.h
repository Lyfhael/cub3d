/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:48:09 by hateisse          #+#    #+#             */
/*   Updated: 2024/03/04 14:08:50 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include "raycasting.h"
# include <stdbool.h>
# include <stdio.h>
# include <sys/fcntl.h>
# include <unistd.h>

# define FAILURE 0
# define SUCCESS 1
# define UNDEFINED ((void *)0)

# define NORTH "NORTH_ID"
# define SOUTH "SOUTH_ID"
# define WEST "WEST_ID"
# define EAST "EAST_ID"
# define FLOOR "FLOOR_ID"
# define CEILING "CEILING_ID"

# define RGB_DELIM ','

# define ALL_TILE_IDS "01NSEW "

enum		e_conf_arg_type
{
	PRIMARY,
	MAP
};

enum		e_tiles
{
	EMPTY = '0',
	WALL = '1',
	GLASS_WALL = '2',
	COLLECTIBLE_A = 'A',
	PLAYER_SPAWN_N = 'N',
	PLAYER_SPAWN_S = 'S',
	PLAYER_SPAWN_E = 'E',
	PLAYER_SPAWN_W = 'W',
	VOID = ' '
};

enum		e_conf_errors
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
	EMPTY_TEXTURE_PATH = -13,
	MAP_NOT_ENCLOSED = -14,
	SPAWN_NOT_FOUND = -15,
	MAP_TILE_BAD_ID = -16,
	MULTIPLE_SPAWNS = -17,
	RGB_INVALID_ZERO = -18
};

enum		e_general_errors
{
	OPEN_FAILED,
	READ_FAILED,
	MALLOC_FAILED
};

enum		e_parse_errors
{
	ARG_NO_INVALID,
	MAP_FILENAME_INVALID,
	INVALID_MAP
};

typedef struct s_map_parsing_info
{
	char	**map;
	int		y;
	int		x;
	int		y_len;
	int		x_len;
	bool	spawn_found;
}			t_map_pinfo;

// main entry
bool		parsing(t_c3d *c3d, int ac, char **ags);

// parsing
bool		_c3d_parse_map(t_c3d *c3d, char **map, t_map *map_info);
char		*_c3d_parse_conf_texture(char *line);
t_rgb		*_c3d_parse_conf_rgb_texture(char *line);
char		*_c3d_parse_rgb(char *rgb_str, t_rgb *result, char delim);
bool		_c3d_parse_arguments(int argc, char **argv);
int			_c3d_expected_conf_arg_type(t_map *map_info);
bool		_c3d_handle_conf_line(char **line, t_map *map_info);
bool		_c3d_try_parse_player_spawn(t_c3d *c3d, char **map, t_map_pinfo *pi);

// save in map_info
bool		_c3d_save_conf_argument(char *line, t_map *map_info);
bool		_c3d_save_texture(char *line, char **texture, char *identifier);
bool		_c3d_save_rgb(char *line, t_rgb **rgb_texture, char *identifier);

// load
bool		_c3d_load_conf_file(char *filepath, t_map *map_info);

//utils
void		_c3d_convert_texture_name_to_perror(char *name);
void		_c3d_premature_gnl_end(int fd, char **line);
bool		_c3d_is_empty_nl(char *line);
void		_c3d_free_map_info(t_map *map_info);
bool		_c3d_is_walkable_tile(char tile);
bool		_c3d_is_player_tile(char tile);
bool		_c3d_walkable_tile_is_enclosed(t_map_pinfo *parse_info);
bool		_c3d_is_obstacle(char tile);
bool		_c3d_is_valid_tile_identifier(t_map_pinfo *pinfo);
void		_c3d_init_t_map_pinfo(t_map_pinfo *parse_info, char **map);
void		_c3d_free_t_c3d(t_c3d *c3d);

// error handling
void		_c3d_parse_perror(int errno);
void		_c3d_conf_file_perror(int errno);
void		_c3d_general_perror(int errno);
void		_c3d_print_usage(void);

//debugging
void		_c3d_print_map_info(t_map *map_info, t_c3d *c3d);

#endif
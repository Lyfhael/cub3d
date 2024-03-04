/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhuang <yzhuang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 08:06:11 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/20 20:47:24 by yzhuang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "raycasting.h"

void	_c3d_print_usage(void)
{
	ft_putstr("Usage: ./cub3d [FILE]\n");
	ft_putstr("The map file needs to be of .cub extension.\n");
}

void	_c3d_general_perror(int errno)
{
	if (errno == OPEN_FAILED)
		perror("cub3d: open() failed");
	else if (errno == READ_FAILED)
		perror("cub3d: read() failed");
	else if (errno == MALLOC_FAILED)
		perror("cub3d: malloc() failed");
}

void	_c3d_map_format_perror(int errno)
{
	if (errno == MAP_NOT_ENCLOSED)
		ft_putstr("Error\nMap is not properly enclosed by walls\n");
	else if (errno == SPAWN_NOT_FOUND)
		ft_putstr("Error\nNo spawn location found in the cub3d map\n");
	else if (errno == MAP_TILE_BAD_ID)
		ft_putstr("Error\nInvalid tile identifiant found in map\n");
	else if (errno == MULTIPLE_SPAWNS)
		ft_putstr("Error\nToo many spawn locations found in the cub3d map\n");
}

void	_c3d_conf_file_perror(int errno)
{
	if (errno == DUPLICATE_NO_TEXTURE)
		ft_putstr("Error\nDuplicate north wall texture in .cub file\n");
	else if (errno == DUPLICATE_SO_TEXTURE)
		ft_putstr("Error\nDuplicate south wall texture in .cub file\n");
	else if (errno == DUPLICATE_WE_TEXTURE)
		ft_putstr("Error\nDuplicate west wall texture in .cub file\n");
	else if (errno == DUPLICATE_EA_TEXTURE)
		ft_putstr("Error\nDuplicate east wall texture in .cub file\n");
	else if (errno == DUPLICATE_FLOOR_TEXTURE)
		ft_putstr("Error\nDuplicate floor texture in .cub file\n");
	else if (errno == DUPLICATE_CEILING_TEXTURE)
		ft_putstr("Error\nDuplicate ceiling color in .cub file\n");
	else if (errno == UNKNOWN_PARAM)
		ft_putstr("Error\nUnknown parameter found in .cub file\n");
	else if (errno == RGB_OVERFLOW)
		ft_putstr("Error\nR,G or B value over 255 in .cub file\n");
	else if (errno == RGB_INVALID_ENDING)
		ft_putstr("Error\nRGB has invalid ending characters in .cub file\n");
	else if (errno == RGB_INVALID)
		ft_putstr("Error\nRGB has invalid characters in .cub file\n");
	else if (errno == EMPTY_TEXTURE_PATH)
		ft_putstr("Error\nPath to texture is empty in .cub file\n");
	else if (errno == RGB_INVALID_ZERO)
		ft_putstr("Error\nR,G or B value not in base 10 in .cub file\n");
}

void	_c3d_parse_perror(int errno)
{
	if (errno == ARG_NO_INVALID)
	{
		ft_putstr("Invalid number of argument.\n");
		_c3d_print_usage();
	}
	else if (errno == MAP_FILENAME_INVALID)
		perror("Invalid filename.");
	else if (errno == INVALID_MAP)
		perror("Invalid map.");
}

bool	_c3d_parse_arguments(int argc, char **ags)
{
	if (argc != 2)
		return (_c3d_parse_perror(ARG_NO_INVALID), FAILURE);
	if (!ft_str_endswith(ags[1], ".cub"))
		return (_c3d_parse_perror(MAP_FILENAME_INVALID), FAILURE);
	return (SUCCESS);
}

int	_c3d_expected_conf_arg_type(t_map *map_info)
{
	if (map_info->textures.ceiling != UNDEFINED
		&& map_info->textures.no_wall != UNDEFINED
		&& map_info->textures.so_wall != UNDEFINED
		&& map_info->textures.we_wall != UNDEFINED
		&& map_info->textures.ea_wall != UNDEFINED
		&& map_info->textures.floor != UNDEFINED)
		return (MAP);
	return (PRIMARY);
}

void	_c3d_convert_texture_name_to_perror(char *name)
{
	if (ft_strcmp(name, NORTH) == 0)
		_c3d_conf_file_perror(DUPLICATE_NO_TEXTURE);
	else if (ft_strcmp(name, SOUTH) == 0)
		_c3d_conf_file_perror(DUPLICATE_SO_TEXTURE);
	else if (ft_strcmp(name, WEST) == 0)
		_c3d_conf_file_perror(DUPLICATE_WE_TEXTURE);
	else if (ft_strcmp(name, EAST) == 0)
		_c3d_conf_file_perror(DUPLICATE_EA_TEXTURE);
	else if (ft_strcmp(name, FLOOR) == 0)
		_c3d_conf_file_perror(DUPLICATE_FLOOR_TEXTURE);
	else if (ft_strcmp(name, CEILING) == 0)
		_c3d_conf_file_perror(DUPLICATE_CEILING_TEXTURE);
}

char	*_c3d_parse_rgb(char *rgb_str, t_rgb *result, char delim)
{
	if (*result > 255)
		return (_c3d_conf_file_perror(RGB_OVERFLOW), NULL);
	else if (ft_isdigit(*rgb_str))
	{
		*result *= 10;
		*result += *rgb_str - 48;
		return (_c3d_parse_rgb(rgb_str + 1, result, delim));
	}
	else if (delim != '\0' && delim != *rgb_str)
		return (_c3d_conf_file_perror(RGB_INVALID), NULL);
	else if (delim != '\0' && delim == *rgb_str)
		return (rgb_str + 1);
	else if (delim == '\0' && delim != *rgb_str && !ft_is_whitespace(*rgb_str))
		return (_c3d_conf_file_perror(RGB_INVALID_ENDING), NULL);
	else
		return (rgb_str);
}

t_rgb	*_c3d_parse_conf_rgb_texture(char *line)
{
	unsigned int	i;
	t_rgb			*rgb;

	line = ft_go_to_next_word(line);
	rgb = ft_calloc(3, sizeof(t_rgb));
	if (!rgb)
		return (NULL);
	i = -1;
	while (++i < 3)
	{
		if (!ft_isdigit(*line))
			return (free(rgb), _c3d_conf_file_perror(RGB_INVALID), NULL);
		if (line[0] == '0' && ft_isdigit(line[1]))
			return (free(rgb), _c3d_conf_file_perror(RGB_INVALID_ZERO), NULL);
		if (i < 2)
			line = _c3d_parse_rgb(line, &rgb[i], RGB_DELIM);
		else
			line = _c3d_parse_rgb(line, &rgb[i], '\0');
		if (!line)
			return (free(rgb), NULL);
	}
	return (rgb);
}

bool	_c3d_save_rgb(char *line, t_rgb **rgb_texture, char *identifier)
{
	if (*rgb_texture != UNDEFINED)
	{
		_c3d_convert_texture_name_to_perror(identifier);
		return (FAILURE);
	}
	else
	{
		*rgb_texture = _c3d_parse_conf_rgb_texture(line);
		if (!*rgb_texture)
			return (FAILURE);
	}
	return (SUCCESS);
}

char	*_c3d_parse_conf_texture(char *line)
{
	char	*texture_path;

	line = ft_go_to_next_word(line);
	if (!*line)
		return (_c3d_conf_file_perror(EMPTY_TEXTURE_PATH), NULL);
	texture_path = ft_dup_next_word(line);
	if (!texture_path)
		return (_c3d_general_perror(MALLOC_FAILED), NULL);
	else if (!*texture_path)
	{
		free(texture_path);
		_c3d_conf_file_perror(EMPTY_TEXTURE_PATH);
		return (NULL);
	}
	else
		return (texture_path);
}

bool	_c3d_save_texture(char *line, char **texture, char *identifier)
{
	if (*texture != UNDEFINED)
	{
		_c3d_convert_texture_name_to_perror(identifier);
		return (FAILURE);
	}
	else
	{
		*texture = _c3d_parse_conf_texture(line);
		if (!*texture)
			return (FAILURE);
	}
	return (SUCCESS);
}

bool	_c3d_save_conf_argument(char *line, t_map *map_info)
{
	if (ft_str_startswith(line, "NO "))
	{
		if (!_c3d_save_texture(line, &(map_info->textures.no_wall), NORTH))
			return (FAILURE);
	}
	else if (ft_str_startswith(line, "SO "))
	{
		if (!_c3d_save_texture(line, &(map_info->textures.so_wall), SOUTH))
			return (FAILURE);
	}
	else if (ft_str_startswith(line, "WE "))
	{
		if (!_c3d_save_texture(line, &(map_info->textures.we_wall), WEST))
			return (FAILURE);
	}
	else if (ft_str_startswith(line, "EA "))
	{
		if (!_c3d_save_texture(line, &(map_info->textures.ea_wall), EAST))
			return (FAILURE);
	}
	else if (ft_str_startswith(line, "F "))
	{
		if (!_c3d_save_rgb(line, &(map_info->textures.floor), FLOOR))
			return (FAILURE);
	}
	else if (ft_str_startswith(line, "C "))
	{
		if (!_c3d_save_rgb(line, &(map_info->textures.ceiling), CEILING))
			return (FAILURE);
	}
	else
		return (_c3d_conf_file_perror(UNKNOWN_PARAM), FAILURE);
	return (SUCCESS);
}

bool	_c3d_handle_conf_line(char **line, t_map *map_info)
{
	int		expected_arg_type;
	char	**tmp;

	if (**line == '\0')
		return (SUCCESS);
	expected_arg_type = _c3d_expected_conf_arg_type(map_info);
	if (expected_arg_type == PRIMARY)
	{
		if (_c3d_save_conf_argument(*line, map_info) == FAILURE)
			return (FAILURE);
	}
	else if (expected_arg_type == MAP)
	{
		tmp = ft_strs_expand(map_info->map, *line);
		if (!tmp)
			return (free(*line), _c3d_general_perror(MALLOC_FAILED), FAILURE);
		*line = NULL;
		map_info->map = tmp;
	}
	else
		return (FAILURE);
	return (SUCCESS);
}

bool	_c3d_is_empty_nl(char *line)
{
	if (line && *line == '\n' && ft_strlen(line) == 1)
		return (true);
	return (false);
}

void	_c3d_premature_gnl_end(int fd, char **line)
{
	gnl_force_finish(1, fd);
	free(*line);
	*line = NULL;
}

bool	_c3d_load_conf_file(char *filepath, t_map *map_info)
{
	int		fd;
	int		gnl_ret;
	char	*gnl_line;

	gnl_line = NULL;
	fd = open(filepath, O_RDONLY);
	if (fd == -1)
		return (_c3d_general_perror(OPEN_FAILED), FAILURE);
	while (1)
	{
		gnl_ret = get_next_line(fd, &gnl_line);
		if (gnl_ret == -1)
		{
			_c3d_premature_gnl_end(fd, &gnl_line);
			return (_c3d_general_perror(READ_FAILED), FAILURE);
		}
		if (gnl_ret == 0 && gnl_line == NULL)
			return (close(fd), SUCCESS);
		if (ft_strlen(gnl_line) > 0
			&& gnl_line[ft_strlen(gnl_line) - 1] == '\n') // a retester
			gnl_line[ft_strlen(gnl_line) - 1] = 0;
		if (_c3d_handle_conf_line(&gnl_line, map_info) == FAILURE)
			return (_c3d_premature_gnl_end(fd, &gnl_line), FAILURE);
		free(gnl_line);
		gnl_line = NULL;
	}
}

void	_c3d_print_map_with_indicator(char **map, int yo, int xo)
{
	int	y;
	int	x;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (y == yo && x == xo)
			{
				ft_putstr("\033[31m");
				ft_putchar(map[y][x]);
				ft_putstr("\033[0m");
			}
			else
				ft_putchar(map[y][x]);
			x++;
		}
		ft_putchar('\n');
		y++;
	}
}

void	_c3d_print_map_info(t_map *map_info, t_pm *pm)
{
	printf("North texture: %s\n", map_info->textures.no_wall);
	printf("South texture: %s\n", map_info->textures.so_wall);
	printf("East texture: %s\n", map_info->textures.ea_wall);
	printf("West texture: %s\n", map_info->textures.we_wall);
	printf("Ceiling color: %d, %d, %d\n",
			map_info->textures.ceiling[0],
			map_info->textures.ceiling[1],
			map_info->textures.ceiling[2]);
	printf("Floor color: %d, %d, %d\n",
			map_info->textures.floor[0],
			map_info->textures.floor[1],
			map_info->textures.floor[2]);
	printf("\n");
	ft_print_array_str(map_info->map);
	printf("\n");
	printf("player position x:%f y:%f\n", pm->player.pos_map.x, pm->player.pos_map.y);
	printf("player angle:%f\n", pm->player.angle);
	printf("map height:%d\n", pm->map.map_size.h);
}

void	_c3d_free_map_info(t_map *map_info)
{
	if (!map_info)
		return ;
	if (map_info->map)
		ft_strsfree(map_info->map);
	if (map_info->textures.no_wall)
		free(map_info->textures.no_wall);
	if (map_info->textures.so_wall)
		free(map_info->textures.so_wall);
	if (map_info->textures.we_wall)
		free(map_info->textures.we_wall);
	if (map_info->textures.ea_wall)
		free(map_info->textures.ea_wall);
	if (map_info->textures.floor)
		free(map_info->textures.floor);
	if (map_info->textures.ceiling)
		free(map_info->textures.ceiling);
	ft_bzero(map_info, sizeof(t_map));
}

bool	_c3d_is_player_tile(char tile)
{
	if (tile == PLAYER_SPAWN_N || tile == PLAYER_SPAWN_S
		|| tile == PLAYER_SPAWN_E || tile == PLAYER_SPAWN_W)
		return (true);
	return (false);
}

bool	_c3d_is_walkable_tile(char tile)
{
	if (tile == EMPTY || tile == COLLECTIBLE_A || _c3d_is_player_tile(tile))
		return (true);
	return (false);
}

bool	_c3d_walkable_tile_is_enclosed(t_map_pinfo *pinfo)
{
	//char	tile;
	int		y;
	int		x;

	y = pinfo->y;
	x = pinfo->x;
	//tile = pinfo->map[y][x];
	if (y == 0 || x == 0 || y == pinfo->y_len - 1 || x == pinfo->x_len - 1
		|| ft_strlen(pinfo->map[y - 1]) - 1 < x
		|| ft_strlen(pinfo->map[y + 1]) - 1 < x)
		return (_c3d_print_map_with_indicator(pinfo->map, y, x), false);
	if (pinfo->map[y - 1][x] != ' '
		&& pinfo->map[y + 1][x] != ' '
		&& pinfo->map[y][x - 1] != ' '
		&& pinfo->map[y][x + 1] != ' ')
		return (true);
	return (_c3d_print_map_with_indicator(pinfo->map, y, x), false);
}

bool	_c3d_is_obstacle(char tile)
{
	if (tile == WALL || tile == GLASS_WALL)
		return (true);
	return (false);
}

void	_c3d_init_t_map_pinfo(t_map_pinfo *parse_info, char **map)
{
	ft_bzero(parse_info, sizeof(t_map_pinfo));
	parse_info->map = map;
	parse_info->y_len = ft_strslen(map);
}

bool	_c3d_is_valid_tile_identifier(t_map_pinfo *pinfo)
{
	char	tile;
	int		y;
	int		x;

	y = pinfo->y;
	x = pinfo->x;
	tile = pinfo->map[y][x];
	if (ft_strchr(ALL_TILE_IDS, tile))
		return (true);
	return (_c3d_print_map_with_indicator(pinfo->map, y, x), false);
}

bool	_c3d_try_parse_player_spawn(t_pm *pm, char **map, t_map_pinfo *pi)
{
	char	tile;

	tile = map[pi->y][pi->x];
	if (_c3d_is_player_tile(tile))
	{
		if (pi->spawn_found)
			return (_c3d_map_format_perror(MULTIPLE_SPAWNS), FAILURE);
		pi->spawn_found = true;
		pm->player.pos_map.x = pi->x;
		pm->player.pos_map.y = pi->y;
		if (tile == 'N')
			pm->player.angle = PI / 2;
		else if (tile == 'S')
			pm->player.angle = 3 * PI / 2;
		else if (tile == 'E')
			pm->player.angle = 0;
		else if (tile == 'W')
			pm->player.angle = PI;
	}
	return (SUCCESS);
}


bool	_c3d_parse_map(t_pm *pm, char **map, t_map *map_info)
{
	t_map_pinfo	parse_info;

	_c3d_init_t_map_pinfo(&parse_info, map);
	map_info->map_size.h = parse_info.y_len;
	while (map && map[parse_info.y])
	{
		parse_info.x = 0;
		parse_info.x_len = ft_strlen(map[parse_info.y]);
		while (map[parse_info.y][parse_info.x])
		{
			if (!_c3d_is_valid_tile_identifier(&parse_info))
				return (_c3d_map_format_perror(MAP_TILE_BAD_ID), FAILURE);
			if (!_c3d_try_parse_player_spawn(pm, map, &parse_info))
				return (FAILURE);
			if (_c3d_is_walkable_tile(map[parse_info.y][parse_info.x])
				&& !_c3d_walkable_tile_is_enclosed(&parse_info))
				return (_c3d_map_format_perror(MAP_NOT_ENCLOSED), FAILURE);
			parse_info.x++;
		}
		parse_info.y++;
	}
	if (!parse_info.spawn_found)
		return (_c3d_map_format_perror(SPAWN_NOT_FOUND), FAILURE);
	return (SUCCESS);
}

void	_c3d_free_t_pm(t_pm *pm)
{
	if (!pm)
		return ;
	_c3d_free_map_info(&pm->map);
}

bool	parsing(t_pm *pm, int ac, char **ags)
{
	t_map	map_info;

	ft_bzero(&map_info, sizeof(map_info));
	if (_c3d_parse_arguments(ac, ags) == FAILURE
		|| _c3d_load_conf_file(ags[1], &map_info) == FAILURE
		|| _c3d_parse_map(pm, map_info.map, &map_info) == FAILURE)
		return (_c3d_free_map_info(&map_info), FAILURE);
	pm->map = map_info;
	_c3d_print_map_info(&map_info, pm);
	return (SUCCESS);
}

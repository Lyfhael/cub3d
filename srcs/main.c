/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 08:06:11 by hateisse          #+#    #+#             */
/*   Updated: 2024/02/07 10:55:57 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	_c3d_print_usage(void)
{
	ft_putstr("Usage: ./cub3d [FILE]\n");
	ft_putstr("The map \file needs to be of .cub extension.\n");
}

void	_c3d_general_perror(int errno)
{
	if (errno == OPEN_FAILED)
		perror("open() failed.");
	else if (errno == READ_FAILED)
		perror("read() failed.");
	else if (errno == MALLOC_FAILED)
		perror("malloc() failed.");
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
}

void	_c3d_parse_perror(int errno)
{
	if (errno == ARG_NO_INVALID)
	{
		perror("Invalid number of argument.");
		_c3d_print_usage();
	}
	else if (errno == MAP_FILENAME_INVALID)
		perror("Invalid filename.");
	else if (errno == INVALID_MAP)
		perror("Invalid map.");
}

bool	_c3d_parse_arguments(int argc, char **argv)
{
	if (argc != 2)
		return (_c3d_parse_perror(ARG_NO_INVALID), FAILURE);
	if (!ft_endswith(argv[1], ".cub"))
		return (_c3d_parse_perror(MAP_FILENAME_INVALID), FAILURE);
	return (SUCCESS);
}

int	_c3d_expected_conf_arg_type(char *line, t_c3d_map *map_info)
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
	if (name == NORTH)
		_c3d_conf_file_perror(DUPLICATE_NO_TEXTURE);
	else if (name == SOUTH)
		_c3d_conf_file_perror(DUPLICATE_SO_TEXTURE);
	else if (name == WEST)
		_c3d_conf_file_perror(DUPLICATE_WE_TEXTURE);
	else if (name == EAST)
		_c3d_conf_file_perror(DUPLICATE_EA_TEXTURE);
	else if (name == FLOOR)
		_c3d_conf_file_perror(DUPLICATE_FLOOR_TEXTURE);
	else if (name == CEILING)
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
		return (_c3d_conf_file_perror(RGB_INVALID_DELIM), NULL);
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
		if (i < 3)
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
		return (_c3d_conf_file_perror(EMPTY_TEXTURE_PATH), NULL);
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

bool	_c3d_save_conf_argument(char *line, t_c3d_map *map_info)
{
	if (ft_str_startswith(line, "NO "))
		if (!_c3d_save_texture(line, &(map_info->textures.no_wall), NORTH))
			return (FAILURE);
	else if (ft_str_startswith(line, "SO "))
		if (!_c3d_save_texture(line, &(map_info->textures.so_wall), SOUTH))
			return (FAILURE);
	else if (ft_str_startswith(line, "WE "))
		if (!_c3d_save_texture(line, &(map_info->textures.we_wall), WEST))
			return (FAILURE);
	else if (ft_str_startswith(line, "EA "))
		if (!_c3d_save_texture(line, &(map_info->textures.ea_wall), EAST))
			return (FAILURE);
	else if (ft_str_startswith(line, "F "))
		if (!_c3d_save_rgb(line, &(map_info->textures.floor), FLOOR))
			return (FAILURE);
	else if (ft_str_startswith(line, "C "))
		if (!_c3d_save_rgb(line, &(map_info->textures.ceiling), CEILING))
			return (FAILURE);
	else
		return (_c3d_conf_file_perror(UNKNOWN_PARAM), FAILURE);
}

bool	_c3d_handle_conf_line(char *line, t_c3d_map *map_info)
{
	int		expected_arg_type;
	char	**tmp;

	expected_arg_type = _c3d_expected_conf_arg_type(line, map_info);
	if (expected_arg_type == PRIMARY)
	{
		if (_c3d_save_conf_argument(line, map_info) == FAILURE)
			return (FAILURE);
	}
	else if (expected_arg_type == MAP)
	{
		tmp = ft_strs_expand(map_info->map, line);
		if (!tmp)
			return (free(line), _c3d_general_perror(MALLOC_FAILED), FAILURE);
		map_info->map = tmp;
	}
}

bool	_c3d_is_empty_nl(char *line)
{
	if (line && *line == '\n' && ft_strlen(line) == 1)
		return (true);
	return (false);
}

void	_c3d_premature_gnl_end(int fd,  char **line)
{
	gnl_force_finish(1, fd);
	free(*line);
	*line = NULL;
}

bool	_c3d_load_conf_file(char *filepath, t_c3d_map *map_info)
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
		if (_c3d_handle_conf_line(gnl_line, map_info) == FAILURE)
			return (_c3d_premature_gnl_end(fd, &gnl_line), FAILURE);
		free(gnl_line);
		gnl_line = NULL;
	}
}

void	_c3d_free_map_info(t_c3d_map map_info)
{
	if (map_info.map)
		ft_strsfree(map_info.map);
	if (map_info.textures.no_wall)
		free(map_info.textures.no_wall);
	if (map_info.textures.so_wall)
		free(map_info.textures.so_wall);
	if (map_info.textures.we_wall)
		free(map_info.textures.we_wall);
	if (map_info.textures.ea_wall)
		free(map_info.textures.ea_wall);
	if (map_info.textures.floor)
		free(map_info.textures.floor);
	if (map_info.textures.ceiling)
		free(map_info.textures.ceiling);
}

int	main(int argc, char **argv)
{
	t_c3d_map	map_info;

	ft_bzero(&map_info, sizeof(map_info));
	if (_c3d_parse_arguments(argc, argv) == FAILURE
		|| _c3d_load_conf_file(argv[1], &map_info) == FAILURE
		|| _c3d_parse_map(&map_info) == FAILURE)
		return (_c3d_free_map_info(map_info), FAILURE);
	_c3d_free_map_info(map_info);
	return (SUCCESS);
}

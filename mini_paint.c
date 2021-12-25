#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <libc.h>
#include <math.h>

typedef struct s_map
{
	int	width;
	int	height;
	char map_char;
} t_map;

typedef struct s_draw
{
	char	in_out;
	float	x;
	float	y;
	float	rad;
	char	character;
} t_draw;

int	ft_strlen(char *string)
{
	int	i;

	i = 0;
	while (string[i] != '\0')
		i++;
	return (i);
}

int	throw_error(int	type)
{
	char *string;

	if (type == 1)
	{
		string = "Error: argument\n";
		write(1, string, ft_strlen(string));
		return (1);
	}
	if (type == 2)
	{
		string = "Error: Operation file corrupted\n";
		write(1, string, ft_strlen(string));
		return (1);
	}
	return (1);
}

char **fill_map(FILE *file, t_map *map, char **f_map)
{
	int	i;
	int	j;
	int ret;
	t_draw draw;
	float	distance;

	i = 0;
	j = 0;
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &draw.in_out, &draw.x, &draw.y, &draw.rad, &draw.character)) == 5)
	{
		if (draw.rad <= 0)
			return (NULL);
		i = 0;
		j = 0;
		if (draw.in_out != 'c' && draw.in_out != 'C')
			return (NULL);
		else if (draw.in_out == 'c')
		{
			while (i < map->height)
			{
				while (j < map->width)
				{
					distance = sqrtf((j - draw.x)*(j - draw.x) + (i - draw.y)*(i - draw.y));
					if (distance <= draw.rad && distance > draw.rad - 1)
						f_map[i][j] = draw.character;
					j++;
				}
				i++;
				j = 0;
			}
		}
		else if (draw.in_out == 'C')
		{
			while (i < map->height)
			{
				while (j < map->width)
				{
					distance = sqrtf((j - draw.x)*(j - draw.x) + (i - draw.y)*(i - draw.y));
					if (distance <= draw.rad)
						f_map[i][j] = draw.character;
						
					j++;
				}
				i++;
				j = 0;
			}
		}
	}
	if (ret == - 1)
		return (f_map);
	return (NULL);
}

int check_back(t_map *map)
{
	if (map->width > 300 || map->width <= 0)
		return (-1);
	if (map->height > 300 || map->height <= 0)
		return (-1);
	return (0);
}


int	main(int argc, char **argv)
{
	FILE	*file;
	t_map	map;
	char	**f_map;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (argc != 2)
		return (throw_error(1));
	file = fopen(argv[1], "r");
	if (!file)
		return(throw_error(2));
	if (fscanf(file, "%d %d %c\n", &map.width, &map.height, &map.map_char) != 3)
		return (throw_error(2));
	if (check_back(&map) == -1)
		return (throw_error(2));
	f_map = malloc(301 * 301);
	while (i < map.height)
	{
		f_map[i] = malloc(301);
		i++;
	}
	i = 0;
	while (i < map.height)
	{
		while (j < map.width)
		{
			f_map[i][j] = map.map_char;
			j++;
		}
		f_map[i][j] = '\n';
		i++;
		j = 0;
	}
	if (!fill_map(file, &map, f_map))
		return (throw_error(2));
	i = 0;
	while (i < map.height)
	{
		write(1, f_map[i], ft_strlen(f_map[i]));
		i++;
	}
	fclose(file);
	return (0);
}
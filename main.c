/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusilanc <jusilanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 22:19:03 by jusilanc          #+#    #+#             */
/*   Updated: 2023/06/25 14:50:50 by jusilanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_base
{
	FILE	*file;
	int		size;
	int		width;
	int		height;
	char	bg_char;
}			t_base;

typedef struct s_circle
{
	char	type;
	float	xpos;
	float	ypos;
	float	radius;
	char	c_char;
}			t_circle;

static int	zone_init(t_base *base)
{
	if (fscanf(base->file, "%i %i %c", &base->width, &base->height,
			&base->bg_char) != 3 || base->width <= 0 || base->width > 300
		|| base->height <= 0 || base->height > 300)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (-1);
	}
	fscanf(base->file, "\n");
	return (0);
}

static void	map_write(t_base *base, char *ptr)
{
	int	y;

	y = 0;
	while (y < base->height)
	{
		write(1, &ptr[y * base->width], base->width);
		write(1, "\n", 1);
		y++;
	}
}

static void	map_init(t_base *base, char *ptr)
{
	int	y;
	int	x;

	y = 0;
	while (y < base->height)
	{
		x = 0;
		while (x < base->width)
		{
			ptr[y * base->width + x] = base->bg_char;
			x++;
		}
		y++;
	}
}

static void	circle_draw(t_base *base, t_circle *circle, char *map)
{
	int		x;
	int		y;
	float	dist;

	y = 0;
	while (y < base->height)
	{
		x = 0;
		while (x < base->width)
		{
			dist = sqrtf((x - circle->xpos) * (x - circle->xpos) + (y
						- circle->ypos) * (y - circle->ypos));
			if (circle->type == 'C' && dist <= circle->radius)
				map[y * base->width + x] = circle->c_char;
			else if (circle->type == 'c' && circle->radius - dist < 1 && circle->radius - dist >= 0)
				map[y * base->width + x] = circle->c_char;
			x++;
		}
		y++;
	}
}

int	main(int argc, char **argv)
{
	t_base		base;
	t_circle	circle;
	char		*map;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	base.file = fopen(argv[1], "r");
	if (zone_init(&base) == -1)
		return (1);
	map = malloc(sizeof(char) * base.width * base.height + 1);
	map_init(&base, map);
	while (1)
	{
		base.size = fscanf(base.file, "%c %f %f %f %c", &circle.type, &circle.xpos, &circle.ypos, &circle.radius, &circle.c_char);
		fscanf(base.file, "\n");
		if ((base.size != 5 && base.size != -1) || (base.size == 5 && circle.type != 'c' && circle.type != 'C') || (base.size != -1 && circle.radius <= 0))
		{
			free(map);
			fclose(base.file);
			write(1, "Error: Operation file corrupted\n", 32);
			return (1);
		}
		else
			circle_draw(&base, &circle, map);
		if (base.size == -1)
			break ;
	}
	map_write(&base, map);
	free(map);
	fclose(base.file);
	return (0);
}

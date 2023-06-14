/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusilanc <jusilanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 22:19:03 by jusilanc          #+#    #+#             */
/*   Updated: 2023/06/14 12:11:18 by jusilanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_base
{
	FILE	*file;
	size_t	size;
	char	ptr[1024];
	int		width;
	int		height;
	char	bg_char;
	char	**image;
	char	circle_type;
	float	pos_x;
	float	pos_y;
	float	radius;
	char	circle_char;
}			t_base;

static void	print_image(t_base *base)
{
	int	i;
	int	j;

	i = 0;
	while (i < base->height)
	{
		j = 0;
		while (j < base->width)
			write(1, &base->image[i][j++], 1);
		i++;
		write(1, "\n", 1);
	}
}

static void	image_init(t_base *base)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	base->image = malloc(sizeof(char *) * base->height + 1);
	while (i < base->height)
	{
		j = 0;
		base->image[i] = malloc(sizeof(char) * base->width + 1);
		while (j < base->width)
			base->image[i][j++] = base->bg_char;
		base->image[i][j] = 0;
		i++;
	}
	base->image[i] = 0;
}

static int	ft_atoi(char *str)
{
	int	n;

	n = 0;
	while (str && *str && *str >= '0' && *str <= '9')
		n = n * 10 + *str++ - 48;
	return (n);
}

static void	move_next_arg(t_base *base)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (base->ptr[i] && base->ptr[i] != ' ')
		// while (base->ptr[i] && (base->ptr[i] != ' ' || base->ptr[i] != '\n'))
		i++;
	i++;
	while (base->ptr[i])
		base->ptr[j++] = base->ptr[i++];
	base->ptr[j++] = base->ptr[i++];
}

static void	get_circle(t_base *base)
{
	base->circle_type = base->ptr[0];
	move_next_arg(base);
	base->pos_x = ft_atoi(base->ptr);
	move_next_arg(base);
	base->pos_y = ft_atoi(base->ptr);
	move_next_arg(base);
	base->radius = ft_atoi(base->ptr);
	move_next_arg(base);
	base->circle_char = base->ptr[0];
	move_next_arg(base);
}

static void	draw_circle(t_base *base)
{
	int	y;
	int	half_row_width;
	int	x;
	int	i;

	i = 0;
	y = -base->radius;
	while (y < base->radius)
	{
		half_row_width = sqrt(base->radius * base->radius - y * y);
		x = -half_row_width;
		while (x < half_row_width)
			base->image[(int)base->pos_x + x++][(int)base->pos_y
				+ y] = base->circle_char;
		y++;
	}
}

static void	zone_init(t_base *base)
{
	int	i;

	i = 0;
	fread(base->ptr, 1024, 1, base->file);
	base->width = ft_atoi(base->ptr);
	move_next_arg(base);
	printf("test[%s]\n", base->ptr);
	fread(base->ptr, 1024, 1, base->file);
	base->height = ft_atoi(base->ptr);
	move_next_arg(base);
	fread(base->ptr, 1024, 1, base->file);
	base->bg_char = base->ptr[0];
	move_next_arg(base);
	printf("get_circle: %s|\n", base->ptr);
}

int	main(int argc, char **argv)
{
	t_base	base;
	size_t	size;

	size = 0;
	(void)argv;
	(void)argc;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	base.file = fopen(argv[1], "r");
	if (!base.file)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	zone_init(&base);
	image_init(&base);
	while (*base.ptr)
	{
		if (base.ptr[0] == 0)
			break ;
		get_circle(&base);
		// printf("here :%s:\n", base.ptr);
		draw_circle(&base);
	}
	print_image(&base);
	fclose(base.file);
	return (0);
}

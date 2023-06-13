/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusilanc <jusilanc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 22:19:03 by jusilanc          #+#    #+#             */
/*   Updated: 2023/06/14 01:14:26 by jusilanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

typedef struct s_base
{
	FILE	*file;
	size_t	size;
	char	ptr[1024];
	int		width;
	int		height;
	char	bg_char;
}			t_base;

static int	ft_atoi(char *str)
{
	int	n;

	n = 0;
	while (str && *str && *str >= '0' && *str <= '9')
		n = n * 10 + *str++ - 48;
	return (n);
}

static void	zone_init(t_base *base)
{
	fread(base->ptr, 1024, 1, base->file);
	base->width = ft_atoi(base->ptr);
	fread(base->ptr, 1024, 1, base->file);
	base->height = ft_atoi(base->ptr);
	fread(base->ptr, 1024, 1, base->file);
	base->bg_char = base->ptr[0];
}

int	main(int argc, char **argv)
{
	t_base	base;
	FILE	*file;
	size_t	size;
	char	ptr[1024];

	size = 0;
	(void)argv;
	(void)argc;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	base.file = fopen(argv[1], "r");
	if (!file)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	zone_init(&base);
	while (1)
	{
		size = fread(ptr, 1024, 3, file);
	}
	fclose(file);
	return (0);
}

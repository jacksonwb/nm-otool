/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 10:41:43 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:06:52 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void		usage(void)
{
	ft_printf("usage nm file1 ...\n");
	exit(0);
}

void		err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

uint32_t	swap_uint32(uint32_t num)
{
	uint32_t	new;
	int			i;

	i = 0;
	while (i < 4)
	{
		((uint8_t*)&new)[3 - (3 - i)] = ((uint8_t*)&num)[3 - i];
		i++;
	}
	return (new);
}

uint32_t	find_native_arch(struct fat_arch *arch, uint32_t nfat_arch,
	int *solo, int big)
{
	uint32_t i;
	uint32_t cputype;

	i = 0;
	while (nfat_arch > 1 && i < nfat_arch)
	{
		cputype = SWAP32((arch[i].cputype), big);
		if (cputype == CPU_TYPE_X86_64)
		{
			*solo = 1;
			return (i);
		}
		i++;
	}
	return (0);
}

void		print_cpu_32(struct fat_arch *arch, char *path, int big)
{
	char		cpu_type[10];
	uint32_t	cputype;

	cputype = SWAP32(arch->cputype, big);
	if (cputype == CPU_TYPE_I386)
		ft_strcpy(cpu_type, "i386");
	else if (cputype == CPU_TYPE_POWERPC)
		ft_strcpy(cpu_type, "ppc");
	else if (cputype == CPU_TYPE_X86_64)
		ft_strcpy(cpu_type, "x86_64");
	ft_printf("%s (for architecture %s):\n", path, cpu_type);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_otool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 21:12:25 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:21:42 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_OTOOL_H
# define FT_NM_OTOOL_H

# include <stdlib.h>
# include <stdio.h>
# include <ar.h>
# include <mach/machine.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include "ft_printf.h"

# define SEG64 struct segment_command_64
# define SEC64 struct section_64
# define SEG32 struct segment_command
# define SEC32 struct section
# define SWAP32(i, big) big ? swap_uint32(i) : i

void		usage(void);
void		err_exit(char *str);
uint32_t	swap_uint32(uint32_t num);
uint32_t	find_native_arch(struct fat_arch *arch, uint32_t nfat_arch,
	int *solo, int big);
void		print_cpu_32(struct fat_arch *arch, char *path, int big);

#endif

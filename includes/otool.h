/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:46:58 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:20:36 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H

# include <fcntl.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <ar.h>
# include <mach/machine.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include "libft.h"
# include "ft_printf.h"
# include "ft_nm_otool.h"

void otool(void *ptr, char *path, size_t len);
void print_section_64(SEC64 *sec, void *ptr);
void print_section_32(SEC32 *sec, void *ptr, int big);
void print_segment_64(SEG64 *seg, void *ptr);
void print_segment_32(SEG32 *seg, void *ptr, int big);
void handle_archive(void *ptr, char *path, size_t len);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:46:48 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/06 21:40:36 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void print_section_64(SEC64 *sec)
{
	void *ptr;

	ptr = sec + sizeof(SEC64);
	ft_printf("%x", *(uint32_t*)ptr);
}

void print_segment_64(SEG64 *seg)
{
	SEC64 *section;
	uint32_t i;

	section = (void*)seg + sizeof(SEG64);
	i = 0;
	while (i < seg->nsects)
	{
		if (!ft_strcmp(section[i].segname, SEG_TEXT) && !ft_strcmp(section[i].sectname, SECT_TEXT))
		{
			print_section_64(&section[i]);
		}
		i++;
	}
}

void handle_64(void *ptr, int endian)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	uint32_t i;

	(void)endian;
	header = (struct mach_header_64*)ptr;
	lc = ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			print_segment_64((SEG64*)lc);
			break;
		}
		i++;
		lc = (void*)lc + lc->cmdsize;
	}
}

void otool(void *ptr, char *path, size_t len)
{
	unsigned magic;

	(void)path;
	(void)len;
	magic = *(int*)ptr;
	if (magic == MH_MAGIC_64)
		handle_64(ptr, 0);
	// else if (magic == MH_MAGIC || magic == MH_CIGAM)
	// 	handle_32(ptr, (magic == MH_CIGAM));
	// else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	// 	handle_fat_32(ptr, path, len, (magic == FAT_CIGAM));
	// else if (!ft_strncmp(ptr, ARMAG, SARMAG))
	// 	handle_archive(ptr, path, len);
}

int main(int ac, char **av)
{
	int i;
	int fd;
	void *ptr;
	struct stat buf;

	if (ac < 2)
		usage();
	i = 1;
	while (i < ac)
	{
		if (ac > 2)
			ft_printf("\n%s:\n", av[i]);
		if ((fd = open(av[i], O_RDONLY)) < 0)
			err_exit(av[i]);
		ft_memset(&buf, 0, sizeof(buf));
		if (fstat(fd, &buf) < 0)
			err_exit("fstat");
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			err_exit("mmap");
		otool(ptr, av[i], buf.st_size);
		if (munmap(ptr, buf.st_size) == -1)
			err_exit(av[i]);
		i++;
	}
	return (0);
}

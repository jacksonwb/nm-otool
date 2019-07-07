/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:46:48 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:19:23 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	handle_64(void *ptr, int endian)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	uint32_t				i;

	(void)endian;
	header = (struct mach_header_64*)ptr;
	lc = ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			print_segment_64((SEG64*)lc, ptr);
		}
		i++;
		lc = (void*)lc + lc->cmdsize;
	}
}

void	handle_32(void *ptr, int endian)
{
	struct mach_header	*header;
	struct load_command	*lc;
	uint32_t			i;

	header = (struct mach_header*)ptr;
	lc = ptr + sizeof(struct mach_header);
	i = 0;
	while (i < (SWAP32(header->ncmds, endian)))
	{
		if ((SWAP32((lc->cmd), endian)) == LC_SEGMENT)
			print_segment_32((SEG32*)lc, ptr, endian);
		i++;
		lc = (void*)lc + (SWAP32(lc->cmdsize, endian));
	}
}

void	handle_fat_32(void *ptr, char *path, size_t len, int big)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	uint32_t			nfat_arch;
	uint32_t			i;
	int					solo;

	header = ptr;
	i = 0;
	(void)len;
	solo = 0;
	arch = (void*)header + sizeof(struct fat_header);
	nfat_arch = SWAP32(header->nfat_arch, big);
	i = find_native_arch(arch, nfat_arch, &solo, big);
	while (i < nfat_arch)
	{
		if (nfat_arch > 1 && !solo)
			print_cpu_32(&arch[i], path, big);
		otool(ptr + (SWAP32(arch[i].offset, big)), path,
			SWAP32(arch[i].size, big));
		if (solo)
			break ;
		i++;
	}
}

void	otool(void *ptr, char *path, size_t len)
{
	unsigned magic;

	(void)path;
	(void)len;
	magic = *(int*)ptr;
	if (magic == MH_MAGIC_64)
		handle_64(ptr, 0);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		handle_32(ptr, (magic == MH_CIGAM));
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		handle_fat_32(ptr, path, len, (magic == FAT_CIGAM));
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
		handle_archive(ptr, path, len);
}

int		main(int ac, char **av)
{
	int			i;
	int			fd;
	void		*ptr;
	struct stat	buf;

	if (ac < 2)
		usage();
	i = 1;
	while (i < ac)
	{
		ft_printf("%s:\n", av[i]);
		if ((fd = open(av[i], O_RDONLY)) < 0)
			err_exit(av[i]);
		ft_memset(&buf, 0, sizeof(buf));
		if (fstat(fd, &buf) < 0)
			err_exit("fstat");
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
			MAP_FAILED)
			err_exit("mmap");
		otool(ptr, av[i], buf.st_size);
		if (munmap(ptr, buf.st_size) == -1)
			err_exit(av[i]);
		i++;
	}
	return (0);
}

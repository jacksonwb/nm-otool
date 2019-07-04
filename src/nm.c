/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 12:53:47 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/03 17:42:43 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void usage(void)
{
	ft_printf("usage nm file1 ...\n");
	exit(0);
}

void err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

char get_sym_type(uint8_t n_type, uint8_t sect, uint32_t *type)
{
	if ((n_type & N_TYPE) == N_UNDF)
		return ('U');
	else if ((n_type & N_TYPE) == N_INDR)
		return ('I');
	else if ((n_type & N_TYPE) == N_ABS)
		return (n_type & N_EXT ? 'A' : 'a');
	else if ((n_type & N_TYPE) == N_SECT && sect == type[S_TEXT])
		return (n_type & N_EXT ? 'T' : 't');
	else if ((n_type & N_TYPE) == N_SECT && sect == type[S_DATA])
		return (n_type & N_EXT ? 'D' : 'd');
	else if ((n_type & N_TYPE) == N_SECT && sect == type[S_BSS])
		return (n_type & N_EXT ? 'B' : 'b');
	else if ((n_type & N_TYPE) == N_SECT)
		return (n_type & N_EXT ? 'S' : 's');
	return (0);
}

void handle_32(void *ptr, int endian)
{
	struct mach_header *header;
	struct load_command *lc;
	uint32_t i;
	uint32_t type[4];

	header = (struct mach_header*)ptr;
	lc = ptr + sizeof(struct mach_header);
	i = 0;
	type[S_TOTAL] = 0;
	while (i < (SWAP32(header->ncmds, endian)))
	{
		if ((SWAP32((lc->cmd), endian)) == LC_SEGMENT)
			segment_32((SEG32*)lc, type, endian);
		if ((SWAP32(lc->cmd, endian)) == LC_SYMTAB)
			symbols_32((struct symtab_command*)lc, ptr, type, endian);
		i++;
		lc = (void*)lc + (SWAP32(lc->cmdsize, 1));
	}
}

void handle_64(void *ptr, int endian)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	uint32_t i;
	uint32_t type[4];

	(void)endian;
	header = (struct mach_header_64*)ptr;
	lc = ptr + sizeof(struct mach_header_64);
	i = 0;
	type[S_TOTAL] = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			segment_64((SEG64*)lc, type);
		if (lc->cmd == LC_SYMTAB)
			symbols_64((struct symtab_command*)lc, ptr, type);
		i++;
		lc = (void*)lc + lc->cmdsize;
	}
}

void handle_archive(void *ptr, char *path, size_t len)
{
	struct ar_hdr *ar;
	char *str;
	uint32_t size;
	void* start;

	start = ptr;
	ar = ptr + SARMAG;
	ptr = (void*)ar + ft_atoi(ar->ar_size) + sizeof(struct ar_hdr);
	while (ptr)
	{
		ar = ptr;
		if (ptr > start + len || ft_atoi(ar->ar_size) <= 0)
			break;
		str = ptr + sizeof(struct ar_hdr);
		ft_printf("\n%s(%s):\n", path, str);
		size = ft_strlen(str);
		while(!*(str + size))
			size++;
		ptr = (void*)str + size;
		nm(ptr, path, len);
		ptr = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	}
}

void print_cpu_32(struct fat_arch *arch, char *path, int big)
{
	char cpu_type[10];
	uint32_t cputype = SWAP32(arch->cputype, big);

	if (cputype == CPU_TYPE_I386)
		ft_strcpy(cpu_type, "i386");
	if (cputype == CPU_TYPE_POWERPC)
		ft_strcpy(cpu_type, "ppc");
	ft_printf("\n%s (for architecture %s):\n", path, cpu_type);
}

void handle_fat_32(void *ptr, char *path, size_t len, int big)
{
	struct fat_header* header;
	struct fat_arch* arch;
	uint32_t i;

	header = ptr;
	i = 0;
	arch = (void*)header + sizeof(struct fat_header);
	while (i < (SWAP32(header->nfat_arch, big)))
	{
		print_cpu_32(&arch[i], path, big);
		nm(ptr + (SWAP32(arch[i].offset, big)), path, len);
		i++;
	}
}

void nm(void *ptr, char *path, size_t len)
{
	unsigned magic;

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

uint32_t swap_uint32(uint32_t num)
{
	uint32_t new;
	int i;

	i = 0;
	while (i < 4)
	{
		((uint8_t*)&new)[3 - (3 - i)] = ((uint8_t*)&num)[3 - i];
		i++;
	}
	return (new);
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
		if ((fd = open(av[i], O_RDONLY)) < 0)
			err_exit(av[i]);
		ft_memset(&buf, 0, sizeof(buf));
		if (fstat(fd, &buf) < 0)
			err_exit("fstat");
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			err_exit("mmap");
		nm(ptr, av[i], buf.st_size);
		if (munmap(ptr, buf.st_size) == -1)
			err_exit(av[i]);
		i++;
	}
	return (0);
}

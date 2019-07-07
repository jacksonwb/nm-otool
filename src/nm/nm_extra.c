/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_extra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 14:47:34 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:05:14 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	get_sym_type(uint8_t n_type, uint8_t sect, uint32_t *type)
{
	if ((n_type & N_TYPE) == N_UNDF && (n_type & N_EXT))
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

void	print_symbols_32(struct nlist *table, char *strings, uint32_t *type,
	t_printbundle bundle)
{
	uint32_t i;
	uint32_t n_sect;
	uint32_t n_type;
	uint32_t n_strx;
	uint32_t n_value;

	i = 0;
	while (i < bundle.nsyms)
	{
		n_strx = SWAP32(table[i].n_un.n_strx, bundle.big);
		n_type = table[i].n_type;
		n_sect = table[i].n_sect;
		n_value = SWAP32(table[i].n_value, bundle.big);
		if (*(strings + n_strx) && get_sym_type(n_type, n_sect, type))
		{
			if (get_sym_type(n_type, n_sect, type) != 'U'
				&& get_sym_type(n_type, n_sect, type) != 'I')
				ft_printf("%.8llx", n_value);
			else
				ft_printf("% 8c", ' ');
			ft_printf(" %c", get_sym_type(n_type, n_sect, type));
			ft_printf(" %s\n", strings + n_strx);
		}
		i++;
	}
}

void	print_symbols_64(struct nlist_64 *table, char *strings, uint32_t nsyms,
	uint32_t *type)
{
	uint32_t	i;
	char		c;

	i = 0;
	while (i < nsyms)
	{
		c = get_sym_type(table[i].n_type, table[i].n_sect, type);
		if (*(strings + table[i].n_un.n_strx) && c)
		{
			if (c != 'U' && c != 'I')
				ft_printf("%.16llx", table[i].n_value);
			else
				ft_printf("% 16c", ' ');
			ft_printf(" %c", c);
			if (c == 'I')
				ft_printf(" %s (indirect for %s)\n", strings +
					table[i].n_un.n_strx,
					strings + table[i].n_un.n_strx);
			else
				ft_printf(" %s\n", strings + table[i].n_un.n_strx);
		}
		i++;
	}
}

void	handle_archive(void *ptr, char *path, size_t len)
{
	struct ar_hdr	*ar;
	char			*str;
	uint32_t		size;
	void			*start;

	start = ptr;
	ar = ptr + SARMAG;
	ptr = (void*)ar + ft_atoi(ar->ar_size) + sizeof(struct ar_hdr);
	while (ptr)
	{
		ar = ptr;
		if (ptr >= start + len || ft_atoi(ar->ar_size) <= 0)
			break ;
		str = ptr + sizeof(struct ar_hdr);
		ft_printf("\n%s(%s):\n", path, str);
		size = ft_strlen(str);
		while (!*(str + size))
			size++;
		ptr = (void*)str + size;
		nm(ptr, path, len);
		ptr = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	}
}

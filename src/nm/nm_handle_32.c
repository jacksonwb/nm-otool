/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_handle_32.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:26:53 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/06 15:58:15 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void print_symbols_32(struct nlist *table, char *strings, uint32_t *type, t_printbundle bundle)
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

void sort_symbols_32(struct nlist *table, char *strings, uint32_t nsyms, int big)
{
	uint32_t i;
	uint32_t j;
	struct nlist tmp;

	i = 1;
	j = 0;
	while (i < nsyms)
	{
		j = i;
		while (j > 0)
		{
			if (ft_strcmp(SORT_STRING32(j, big), SORT_STRING32(j - 1, big)) < 0)
			{
				tmp = table[j];
				table[j] = table[j - 1];
				table[j - 1] = tmp;
			}
			j--;
		}
		i++;
	}
}

struct nlist *cpy_symbols_32(struct nlist* table, uint32_t nsyms)
{
	struct nlist *cpy;
	uint32_t i;

	cpy = malloc(sizeof(struct nlist) * nsyms);
	i = 0;
	while (i < nsyms)
	{
		ft_memcpy(&cpy[i], &table[i], sizeof(struct nlist));
		i++;
	}
	return (cpy);
}

void symbols_32(struct symtab_command *sym, void *ptr, uint32_t *type, int big)
{
	char *strings;
	struct nlist *table;
	struct nlist *cpy;
	uint32_t i;
	t_printbundle bundle;

	table = ptr + (SWAP32(sym->symoff, big));
	strings = ptr + (SWAP32(sym->stroff, big));
	i = 0;
	cpy = cpy_symbols_32(table, SWAP32(sym->nsyms, big));
	sort_symbols_32(cpy, strings, SWAP32(sym->nsyms, big), big);
	bundle.big = big;
	bundle.nsyms = SWAP32(sym->nsyms, big);
	print_symbols_32(cpy, strings, type, bundle);
	free(cpy);
}

void segment_32(SEG32 *seg, uint32_t *type, int big)
{
	SEC32 *section;
	uint32_t i;

	section = (void*)seg + sizeof(SEG32);
	i = 0;
	while (i < (SWAP32(seg->nsects, big)))
	{
		if (!ft_strcmp(section[i].segname, SEG_TEXT) && !ft_strcmp(section[i].sectname, SECT_TEXT))
			type[S_TEXT] = type[S_TOTAL] + 1;
		else if (!ft_strcmp(section[i].segname, SEG_DATA) && !ft_strcmp(section[i].sectname, SECT_DATA))
			type[S_DATA] = type[S_TOTAL] + 1;
		else if (!ft_strcmp(section[i].segname, SEG_DATA) && !ft_strcmp(section[i].sectname, SECT_BSS))
			type[S_BSS] = type[S_TOTAL] + 1;
		type[S_TOTAL]++;
		i++;
	}
}

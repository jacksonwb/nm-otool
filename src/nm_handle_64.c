/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_handle_64.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:26:53 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/03 13:49:09 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void print_symbols_64(struct nlist_64 *table, char *strings, uint32_t nsyms, uint32_t *type)
{
	uint32_t i;

	i = 0;
	while (i < nsyms)
	{
		if (*(strings + table[i].n_un.n_strx) && get_sym_type(table[i].n_type, table[i].n_sect, type))
		{
			if (get_sym_type(table[i].n_type, table[i].n_sect, type) != 'U')
				ft_printf("%.16llx", table[i].n_value);
			else
				ft_printf("% 16c", ' ');
			ft_printf(" %c", get_sym_type(table[i].n_type, table[i].n_sect, type));
			ft_printf(" %s\n", strings + table[i].n_un.n_strx);
		}
		i++;
	}
}

void sort_symbols_64(struct nlist_64 *table, char *strings, uint32_t nsyms)
{
	uint32_t i;
	uint32_t j;
	struct nlist_64 tmp;

	i = 1;
	j = 0;
	while (i < nsyms)
	{
		j = i;
		while (j > 0)
		{
			if (ft_strcmp(SORT_STRING(j), SORT_STRING(j - 1)) < 0)
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

struct nlist_64 *cpy_symbols_64(struct nlist_64* table, uint32_t nsyms)
{
	struct nlist_64 *cpy;
	uint32_t i;

	cpy = malloc(sizeof(struct nlist_64) * nsyms);
	i = 0;
	while (i < nsyms)
	{
		ft_memcpy(&cpy[i], &table[i], sizeof(struct nlist_64));
		i++;
	}
	return (cpy);
}

void symbols_64(struct symtab_command *sym, void *ptr, uint32_t *type)
{
	char *strings;
	struct nlist_64 *table;
	struct nlist_64 *cpy;
	uint32_t i;

	table = ptr + sym->symoff;
	strings = ptr + sym->stroff;
	i = 0;
	cpy = cpy_symbols_64(table, sym->nsyms);
	sort_symbols_64(cpy, strings, sym->nsyms);
	print_symbols_64(cpy, strings, sym->nsyms, type);
	free(cpy);
}

void segment_64(SEG64 *seg, uint32_t *type)
{
	SEC64 *section;
	uint32_t i;

	section = (void*)seg + sizeof(SEG64);
	i = 0;
	while (i < seg->nsects)
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

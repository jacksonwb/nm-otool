/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 21:02:43 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/06 21:14:09 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <ar.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach/machine.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include "libft.h"
# include "ft_printf.h"
# include "ft_nm_otool.h"

# define S_TEXT 0
# define S_DATA 1
# define S_BSS 2
# define S_TOTAL 3

# define SWAP32(i, big) big ? swap_uint32(i) : i
# define SORT_STRING32(i, big) strings + (SWAP32(table[i].n_un.n_strx, big))
# define SORT_STRING(i) strings + table[i].n_un.n_strx

typedef struct	s_printbundle
{
	uint32_t nsyms;
	int big;
}				t_printbundle;

void nm(void *ptr, char *path, size_t len);
char get_sym_type(uint8_t n_type, uint8_t sect, uint32_t *type);
uint32_t swap_uint32(uint32_t num);

/*
**64 Bit Handlers
*/
void print_symbols_64(struct nlist_64 *table, char *strings, uint32_t nsyms, uint32_t *type);
void sort_symbols_64(struct nlist_64 *table, char *strings, uint32_t nsyms);
struct nlist_64 *cpy_symbols_64(struct nlist_64* table, uint32_t nsyms);
void symbols_64(struct symtab_command *sym, void *ptr, uint32_t *type);
void segment_64(SEG64 *seg, uint32_t *type);

/*
**32 Bit Handlers
*/
void print_symbols_32(struct nlist *table, char *strings, uint32_t *type, t_printbundle bundle);
void sort_symbols_32(struct nlist *table, char *strings, uint32_t nsyms, int big);
struct nlist *cpy_symbols_32(struct nlist* table, uint32_t nsyms);
void symbols_32(struct symtab_command *sym, void *ptr, uint32_t *type, int big);
void segment_32(SEG32 *seg, uint32_t *type, int big);

# endif
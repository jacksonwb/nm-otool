/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 21:02:32 by jbeall            #+#    #+#             */
/*   Updated: 2019/07/07 15:16:25 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	print_section_64(SEC64 *sec, void *ptr)
{
	uint8_t		*text;
	uint32_t	i;

	text = ptr + sec->offset;
	i = 0;
	ft_printf("Contents of (%s,%s) section\n", sec->segname, sec->sectname);
	while (i < sec->size)
	{
		if (i % 16 == 0)
			ft_printf("%.16llx	", sec->addr + i);
		ft_printf("%.2x ", text[i]);
		if (i % 16 == 15)
			ft_printf("\n");
		i++;
	}
	if ((i - 1) % 16 != 15)
		ft_printf("\n");
}

void	print_section_32(SEC32 *sec, void *ptr, int big)
{
	uint8_t		*text;
	uint32_t	i;

	text = ptr + (SWAP32(sec->offset, big));
	i = 0;
	ft_printf("Contents of (%s,%s) section\n", sec->segname, sec->sectname);
	while (i < (SWAP32(sec->size, big)))
	{
		if (i % 16 == 0)
			ft_printf("%.8llx	", (SWAP32(sec->addr, big)) + i);
		ft_printf("%.2x", text[i]);
		if (!big || (big && i % 4 == 3))
			ft_printf(" ");
		if (i % 16 == 15)
			ft_printf("\n");
		i++;
	}
	if ((i - 1) % 16 != 15)
		ft_printf("\n");
}

void	print_segment_64(SEG64 *seg, void *ptr)
{
	SEC64		*section;
	uint32_t	i;

	section = (void*)seg + sizeof(SEG64);
	i = 0;
	while (i < seg->nsects)
	{
		if (!ft_strcmp(section[i].segname, SEG_TEXT) &&
			!ft_strcmp(section[i].sectname, SECT_TEXT))
		{
			print_section_64(&section[i], ptr);
			break ;
		}
		i++;
	}
}

void	print_segment_32(SEG32 *seg, void *ptr, int big)
{
	SEC32		*section;
	uint32_t	nsects;
	uint32_t	i;

	section = (void*)seg + sizeof(SEG32);
	nsects = SWAP32(seg->nsects, big);
	i = 0;
	while (i < nsects)
	{
		if (!ft_strcmp(section[i].segname, SEG_TEXT) &&
			!ft_strcmp(section[i].sectname, SECT_TEXT))
		{
			print_section_32(&section[i], ptr, big);
			break ;
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
		ft_printf("%s(%s):\n", path, str);
		size = ft_strlen(str);
		while (!*(str + size))
			size++;
		ptr = (void*)str + size;
		otool(ptr, path, len);
		ptr = (void*)ar + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size);
	}
}

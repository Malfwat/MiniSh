/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:28:37 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 15:21:25 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdbool.h>
# include <stdarg.h>

# define FORMAT "cspdiuxX%"
# define FLAG "-+ #0."

# include "libft.h"

enum	e_case
{
	empty,
	lower,
	upper
};

enum	e_align
{
	right = 1,
	left
};

enum	e_signed
{
	none,
	wspace,
	pos,
	neg,
	prefix
};

enum	e_format
{
	c,
	s,
	p,
	d,
	i,
	u,
	x,
	X,
	percent
};

enum	e_flag
{
	minus,
	plus,
	space,
	hash,
	zero,
	dot
};

typedef struct s_modif
{
	char			format;
	int				precision;
	int				width;
	char			filling;
	enum e_signed	prefix;
	bool			dot;
	int				align;
}					t_modif;

int		format(char *str, va_list lst);
int		get_modifiyer(char modifiyer[256], char *format);
t_modif	initialize_t_modif(void);
void	analyse(t_modif *rule, char const *str, int *i);
t_modif	analyse_modifiyer(char const *str);
t_modif	*get_rule(void);
int		*get_nb_printed(void);
void	get_sizes(t_modif *rule, char const *str, int *i);
int		ft_putchar(char c);
int		ft_putstr(char *str);
int		fill(int size, char c);
int		putstr_size(char *str, int size);
int		ui_len(va_list lst, char const type);
int		i_len(va_list lst, char const type);
int		s_len(va_list lst, char const type);
int		get_len(char const *type, ...);
int		biggest(int a, int b);
int		ft_printf(char *format, ...) __attribute__((format(printf, 1, 2)));
int		get_number(va_list ap);
int		p_size(unsigned long int tmp, int base_len);
int		arg_size(long long int nb, int base_len, char const type);
int		is_in_str(char c, char *format);
int		get_val(const char *nptr, int *i);
int		ft_putnbr(int nb);
void	print_addr(unsigned long addr);
int		ft_puthexa(long long int nb, enum e_case casee);
int		ft_printnbr(unsigned long int nb, unsigned int base_len, char *base);
int		mesure_arg(char const *str, va_list lst);
void	call_func(char *str, va_list lst);
void	left_align(char *str, va_list lst, int nb_char);
void	right_align(char *str, va_list lst, int nb_char);
void	get_sizes(t_modif *rule, char const *str, int *i);

#endif /*LIBFTPRINTF_H*/

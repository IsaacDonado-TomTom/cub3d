/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isascii.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: idonado <isaac@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 19:42:04 by idonado       #+#    #+#                 */
/*   Updated: 2021/08/21 17:51:26 by idonado       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 0177)
		return (1);
	else
		return (0);
}

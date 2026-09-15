/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 22:11:20 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/14 22:21:03 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	parsing_atoi(char *arg)
{
	int	i;

	i = 0;
	if (!arg[0])
		return (-1);
	while (arg[i])
	{
		if (!is_digit(arg[i]))
			return (-1);
		i++;
	}
	return (atoi(arg));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:25:20 by egoh              #+#    #+#             */
/*   Updated: 2026/05/15 16:25:50 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBC_H
# define LIBC_H

# include <stddef.h>

int		ft_atoi(const char *s);
void	ft_bzero(void *s, size_t len);
void	*ft_calloc(size_t amt, size_t size);
int		ft_isdigit(int c);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_putnbr_fd(int n, int fd);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s, int del_nl);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strtrim(char const *s1, char const *set);
#endif
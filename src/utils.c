#include "../include/cub3d.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	len = strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strtrim(char *str)
{
	int		start;
	int		end;
	int		i;
	char	*trimmed;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && (str[start] == ' ' || str[start] == '\t'))
		start++;
	end = strlen(str) - 1;
	while (end > start && (str[end] == ' ' || str[end] == '\t'
			|| str[end] == '\n'))
		end--;
	trimmed = malloc(end - start + 2);
	if (!trimmed)
		return (str);
	i = 0;
	while (start <= end)
	{
		trimmed[i] = str[start];
		i++;
		start++;
	}
	trimmed[i] = '\0';
	free(str);
	return (trimmed);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*get_word(char const *s, char c, int *index)
{
	int		start;
	int		len;
	char	*word;
	int		i;

	start = *index;
	len = 0;
	while (s[start + len] && s[start + len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	}
	word[i] = '\0';
	*index = start + len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;
	int		i;
	int		index;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		while (s[index] && s[index] == c)
			index++;
		result[i] = get_word(s, c, &index);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

void	free_split(char **split)
{
	int i;

	if (!split)
		return ;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
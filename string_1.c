#include "shell.h"

/**
 * _strlen - returns length of string
 * @s: string whose length to check
 * Return: integer length of string
 */

int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	while (*s++)
		i++;
	return (i);
}

/**
 * _strcmp - performs lexicogarphic comparison of two strangs.
 * @s1: first string
 * @s2: second string
 * Return: negative if s1 < s2, positive if s1 > s2, 0 if s1 == s2
 */

int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * starts_with - checks if string starts with character
 * @string: string to search
 * @character: character to find
 * Return: address of next character of string or NULL
 */

char *starts_with(const char *string, const char *character)
{
	while (*character)
		if (*character++ != *string++)
			return (NULL);
	return ((char *)string);
}

/**
 * _strcat - concatenates two strings
 * @dest: destination buffer
 * @src: source buffer
 * Return: pointer to destination buffer
 */

char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (ret);
}

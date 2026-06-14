/*
 *  linux/lib/string.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * stupid library routines.. The optimized versions should generally be
 * found as inline code in <asm-xx/string.h>
 *
 * These are buggy as well..
 *
 * * Fri Jun 25 1999, Ingo Oeser <ioe@informatik.tu-chemnitz.de>
 * - Added strsep() which will replace strtok() soon (because strsep() is
 * reentrant and should be faster). Use only strsep() in new code, please.
 *
 * * Sat Feb 09 2002, Jason Thomas <jason@topic.com.au>,
 * Matthew Hawkins <matt@mh.dropbear.id.au>
 * -  Kissed strtok() goodbye
 */

#include "sstring.h"

/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */

char *sstrcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}

int sstrcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}

	return 0;
}

/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int sstrncmp(const char *cs, const char *ct, unsigned int count)
{
	unsigned char c1, c2;

	while (count) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}

	return 0;
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
unsigned int sstrlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		;

	return sc - s;
}


void *mmemset(void *s, int c, unsigned int count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;

	return s;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *mmemcpy(void *dest, const void *src, unsigned int count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

int mmemcmp(const void *cs, const void *ct, unsigned int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; count > 0; ++su1, ++su2, count--) {
		res = *su1 - *su2;
		if (res != 0)
			break;
	}

	return res;
}

void memsetw(void *s, int c, unsigned int count)
{
	unsigned int *ptr = (unsigned int *)s;
	count = count >> 2;
	while (count--) {
		*ptr++ = c;
	}
}


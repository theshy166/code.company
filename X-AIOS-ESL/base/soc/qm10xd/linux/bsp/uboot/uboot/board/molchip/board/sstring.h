#ifndef _STRING_H_
#define _STRING_H_

char *sstrcpy(char *dest, const char *src);

int sstrcmp(const char *cs, const char *ct);

int sstrncmp(const char *cs, const char *ct, unsigned int count);

unsigned int sstrlen(const char *s);

void *mmemset(void *s, int c, unsigned int count);

void *mmemcpy(void *dest, const void *src, unsigned int count);

int mmemcmp(const void *cs, const void *ct, unsigned int count);

void memsetw(void *s, int c, unsigned int count);

#endif

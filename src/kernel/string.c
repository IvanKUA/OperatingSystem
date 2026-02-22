#include "string.h"
#include <stddef.h>

const char* strchr(const char* str, char chr)
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

char* strcpy(char* dst, const char* src)
{
    char* origDst = dst;

    if (dst == NULL)
        return NULL;

    if (src == NULL)
    {
        *dst = '\0';
        return dst;
    }

    while (*src)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
    
    *dst = '\0';
    return origDst;
}

unsigned strlen(const char* str)
{
    unsigned len = 0;
    while (*str)
    {
        ++len;
        ++str;
    }

    return len;
}

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (unsigned char)*str1 - (unsigned char)*str2;
}

int strncmp(const char* str1, const char* str2, unsigned n)
{
    while (n && *str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
        n--;
    }

    if (n == 0)
        return 0;

    return (unsigned char)*str1 - (unsigned char)*str2;
}

char* strncpy(char* dst, const char* src, unsigned n)
{
    unsigned i = 0;

    while (i < n && src[i])
    {
        dst[i] = src[i];
        i++;
    }

    while (i < n)
    {
        dst[i] = '\0';
        i++;
    }

    return dst;
}

static int is_delim(char c, const char* delim)
{
    while (*delim)
    {
        if (c == *delim)
            return 1;
        delim++;
    }
    return 0;
}

char* strtok(char* str, const char* delim)
{
    static char* next = 0;

    if (str)
        next = str;

    if (!next)
        return 0;

    while (*next && is_delim(*next, delim))
        next++;

    if (*next == '\0')
    {
        next = 0;
        return 0;
    }

    char* token = next;

    while (*next && !is_delim(*next, delim))
        next++;

    if (*next)
    {
        *next = '\0';
        next++;
    }
    else
    {
        next = 0;
    }

    return token;
}
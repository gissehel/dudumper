#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_utils.h"
#include "mem_utils.h"

int __needsquotes(const char *s, int *quotelen)
{
    int len = strlen(s);
    int index;
    int needsquotes = 0;
    *quotelen = len;
    for (index = 0; index < len; index++)
    {
        if (s[index] == '\'')
        {
            (*quotelen)++;
            needsquotes++;
        }
    }
    return needsquotes;
}

char *quotestrdup_sql(const char *value)
{
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0)
    {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL)
    {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++)
    {
        r = *t;
        if (r == '\'')
        {
            *u++ = r; /* double the quote */
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *quotestrdup_backquote(const char *value)
{
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0)
    {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL)
    {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++)
    {
        r = *t;
        if (r == '\'')
        {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *doublequotestrdup_backquote(const char *value)
{
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0)
    {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL)
    {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++)
    {
        r = *t;
        if (r == '"')
        {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *replace_string(const char *path, const char *directory, const char *display_directory)
{
    if (display_directory == NULL)
    {
        return MEM_STRDUP(path);
    }
    else
    {
        const int path_size = strlen(path);
        const int directory_size = strlen(directory);
        const int display_directory_size = strlen(display_directory);
        const int new_string_size = path_size - directory_size + display_directory_size;
        char *new_string = MEM_CALLOC(new_string_size + 1, char);

        int index = 0;
        for (index = 0; index < display_directory_size; index++)
        {
            new_string[index] = display_directory[index];
        }

        for (; index < new_string_size; index++)
        {
            new_string[index] = path[index - display_directory_size + directory_size];
        }
        return new_string;
    }
}

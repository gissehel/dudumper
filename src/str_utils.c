#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_utils.h"
#include "mem_utils.h"

int __needsquotes(const char* s, int* quotelen) {
    int len = strlen(s);
    int index;
    int needsquotes = 0;
    *quotelen = len;
    for (index=0;index<len;index++) {
        if (s[index] == '\'') {
            (*quotelen)++;
            needsquotes++;
        }
    }
    return needsquotes;
}

char* quotestrdup_sql(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char* t;

    if(__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen+1, char);
    if(ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t=value; *t; t++) {
        r = *t;
        if(r == '\'')  {
            *u++ = r;	/* double the quote */
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char* quotestrdup_backquote(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char* t;

    if(__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }
	
    ret = MEM_CALLOC(quotelen+1, char);
    if(ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t=value; *t; t++) {
        r = *t;
        if(r == '\'')  {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char* doublequotestrdup_backquote(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char* t;

    if(__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }
	
    ret = MEM_CALLOC(quotelen+1, char);
    if(ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t=value; *t; t++) {
        r = *t;
        if(r == '"')  {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

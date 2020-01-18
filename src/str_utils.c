#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "str_utils.h"
#include "mem_utils.h"

int __needsquotes(const char *s, int *quotelen) {
    int len = strlen(s);
    int index;
    int needsquotes = 0;
    *quotelen = len;
    for (index = 0; index < len; index++) {
        if (s[index] == '\'') {
            (*quotelen)++;
            needsquotes++;
        }
    }
    return needsquotes;
}

char *quotestrdup_sql(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++) {
        r = *t;
        if (r == '\'') {
            *u++ = r; /* double the quote */
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *quotestrdup_backquote(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++) {
        r = *t;
        if (r == '\'') {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *doublequotestrdup_backquote(const char *value) {
    char *u, *ret;
    int quotelen;
    char r;

    const char *t;

    if (__needsquotes(value, &quotelen) == 0) {
        return MEM_STRDUP(value);
    }

    ret = MEM_CALLOC(quotelen + 1, char);
    if (ret == NULL) {
        return NULL;
    }
    u = ret;
    for (t = value; *t; t++) {
        r = *t;
        if (r == '"') {
            *u++ = '\\';
        }
        *u++ = r;
    }
    *u = '\0';
    return ret;
}

char *replace_string(const char *path, const char *directory, const char *display_directory, bool display_as_win) {
    if (display_directory == NULL) {
        return MEM_STRDUP(path);
    } else {
        const int path_size = strlen(path);
        const int directory_size = strlen(directory);
        const int display_directory_size = strlen(display_directory);
        int pathsep_size = 0;
        int index = 0;

        if (display_as_win) {
            for (index = 0; index < display_directory_size; index++) {
                if (display_directory[index] == '/') {
                    pathsep_size++;
                }
            }
            for (; index < path_size - directory_size + display_directory_size; index++) {
                if (path[index - display_directory_size + directory_size] == '/') {
                    pathsep_size++;
                }
            }
        }

        const int new_string_size = path_size - directory_size + display_directory_size + pathsep_size;
        char *new_string = MEM_CALLOC(new_string_size + 1, char);
        int pathsep_index = 0;

        for (index = 0; index < display_directory_size; index++) {
            char c = display_directory[index];
            if (display_as_win && c == '/') {
                new_string[index+pathsep_index] = '\\';
                pathsep_index++;
                new_string[index+pathsep_index] = '\\';
            } else {
                new_string[index+pathsep_index] = c;
            }
        }

        for (; index < new_string_size-pathsep_size; index++) {
            char c = path[index - display_directory_size + directory_size];
            if (display_as_win && c == '/') {
                new_string[index+pathsep_index] = '\\';
                pathsep_index++;
                new_string[index+pathsep_index] = '\\';
            } else {
                new_string[index+pathsep_index] = c;
            }
        }
        new_string[index+pathsep_index] = '\0';
        return new_string;
    }
}

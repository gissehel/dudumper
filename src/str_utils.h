#ifndef __STR_UTILS_H
#define __STR_UTILS_H
#include <stdbool.h>

char* quotestrdup_sql(const char *s);
char* quotestrdup_backquote(const char *s);
char* doublequotestrdup_backquote(const char *s);
char* replace_string(const char* path, const char* directory, const char* display_directory, bool display_as_win);

#endif /* __STR_UTILS_H */

#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <stdbool.h>

#include "node_parser.h"
#include "node_info.h"

struct stat st_stat_buf;
struct node_info* node_info_root;
struct node_info* node_info_last;
static struct node_parse_data* node_parse_data;


#define stat_buf (&st_stat_buf)
void dump_node(struct node_info* node_info_item, bool recurse);

bool is_sub_path(const char* value, const char* pattern) {
    size_t value_len = strlen(value);
    size_t pattern_len = strlen(pattern);

    // printf("is sub path [%s] [%s]\n", value, pattern);

    if (value_len < pattern_len+1) {
        return false;
    }

    if (strncmp(value, pattern, pattern_len) == 0) {
        if (value[pattern_len] == '/') {
            size_t index;
            for(index = pattern_len+1; index < value_len; index++) {
                if (value[index] == '/') {
                    /* This is not a direct sub path... like /foo/bar and /foo/bar/sha/bok */
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    return false;
}

char* get_path_name(const char* parent, const char* path) {
    /*
    printf("    z : [%s]\n", parent);
    printf("    y : [%s]\n", path);
    */
    return strdup(path+strlen(parent)+1);
}

/*
void on_node_parse_start() {
    printf("BEGIN TRANSACTION;\n");
    printf("CREATE TABLE IF NOT EXISTS Files(id PRIMARY KEY ON CONFLICT FAIL,name, path, depth, parent_id, size, occ_size);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_path ON Files (path);\n");
    printf("CREATE INDEX IF NOT EXISTS index_Files_parent_id ON Files (id, parent_id);\n");
}

void on_node_parse_stop() {
    // printf("-- end\n");
    // dump_node(node_info_root, true);
    printf("COMMIT;\n");
}
void on_node_display(struct node_info* node_info_item) {
    char* name = quotestrdup(node_info_item->name);
    char* path = quotestrdup(node_info_item->path);
    printf("INSERT INTO Files (id, name, path, depth, parent_id, size, occ_size) VALUES (%ld, '%s', '%s', %ld, %ld, %ld, %ld);\n",  node_info_item->id, name, path, node_info_item->depth, node_info_item->parent == NULL ? 0 : node_info_item->parent->id, node_info_item->size, node_info_item->occ_size);
    free(name);
    free(path);
}
*/



void dump_node_paths() {
    struct node_info* node_info = node_parse_data->node_info_last;
    while (node_info != NULL) {
        node_parse_data->output->on_node_display(node_info);
        node_info = node_info->parent;
    }
}

void dump_node(struct node_info* node_info_item, bool recurse) {
    node_parse_data->output->on_node_display(node_info_item);
    if (recurse) {
        struct node_info *child = node_info_item->first_child;
        while (child != NULL) {
            dump_node(child, recurse);
            child = child->next;
        }
    }
}

void node_info_release() {
    struct node_info* last_node_info = node_parse_data->node_info_last;

    /*
    printf("free ");
    */
    dump_node(node_parse_data->node_info_last, false);
    
    node_parse_data->node_info_last = node_parse_data->node_info_last->parent;
    // printf("-- freeing path [%s]\n", last_node_info->path);
    node_info_free(last_node_info);
}

void node_info_release_all() {
    while (node_parse_data->node_info_last != NULL) {
        node_info_release();
    }
}

int on_file_item(const char* fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    char* name = NULL; 

    while ( (node_parse_data->node_info_last != NULL) && (! is_sub_path(fpath, node_parse_data->node_info_last->path)) ) {
        node_info_release();
    }

    lstat(fpath, stat_buf);

    if (node_parse_data->node_info_last != NULL) {
        if (is_sub_path(fpath, node_parse_data->node_info_last->path)) {
            name = get_path_name(node_parse_data->node_info_last->path, fpath);
        } else {
            name = strdup(fpath);
        }
    } else {
        name = strdup(fpath);
    }
    
    struct node_info* node_info = node_info_create_from_parent(node_parse_data->node_info_last == NULL ? NULL : node_parse_data->node_info_last, fpath, name);

    size_type size = stat_buf->st_size;
    size_type occ_size = stat_buf->st_blksize*stat_buf->st_blocks/8;

    {
        struct node_info* current_node_info = node_info;
        while (current_node_info != NULL) {
            current_node_info->size += size;
            current_node_info->occ_size += occ_size;
            current_node_info = current_node_info->parent;
        }
    }

    if (node_parse_data->node_info_root == NULL) {
        node_parse_data->node_info_root = node_info;
    }
    node_parse_data->node_info_last = node_info;

    free(name);

    return 0;
}


void node_parse(struct node_parse_data* data, const char* path) {
    node_parse_data = data;
    node_parse_data->output->on_node_parse_start();
    nftw(path, on_file_item, 200, 0);
    node_info_release_all();
    node_parse_data->output->on_node_parse_stop();
}

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

char* quotestrdup(const char *value) {
	char *u, *ret;
	int quotelen;
	char r;

    const char* t;

	if(__needsquotes(value, &quotelen) == 0) {
		return strdup(value);
    }
	
	ret = malloc(quotelen+1);
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

struct node_parse_output* node_parse_output_create() {
    struct node_parse_output* output = malloc(sizeof(struct node_parse_output()));
    output->on_node_parse_start = NULL;
    output->on_node_parse_stop = NULL;
    output->on_node_display = NULL;
    return output;
}
struct node_parse_data* node_parse_data_create(const struct node_parse_output* node_parse_output) {
    struct node_parse_data* node_parse_data = malloc(sizeof(struct node_parse_data()));
    node_parse_data->node_info_root = NULL;
    node_parse_data->node_info_last = NULL;
    node_parse_data->output = node_parse_output;
    return node_parse_data;
}


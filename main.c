#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <stdbool.h>
#include "node_info.h"

struct stat st_stat_buf;
struct node_info* node_info_root;
struct node_info* node_info_last;

#define stat_buf (&st_stat_buf)

bool is_sub_path(const char* value, const char* pattern) {
    size_t value_len = strlen(value);
    size_t pattern_len = strlen(pattern);

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

void dump_node_paths() {
    struct node_info* node_info = node_info_last;
    while (node_info != NULL) {
        printf("    node path : [%s] [%ld] [%ld]\n", node_info->path, node_info->size, node_info->occ_size);
        node_info = node_info->parent;
    }
}

void dump_node(struct node_info* node_info_item, bool recurse);
void dump_node(struct node_info* node_info_item, bool recurse) {
    int index_width;
    for (index_width = 0; index_width<node_info_item->depth; index_width++) {
        printf("  ");
    }
    printf("[%s] [%ld] [%ld]\n", node_info_item->name, node_info_item->size, node_info_item->occ_size);
    struct node_info *child = node_info_item->first_child;
    if (recurse) {
        while (child != NULL) {
            dump_node(child, recurse);
            child = child->next;
        }
    }
}

void node_info_release() {
    struct node_info* last_node_info = node_info_last;

    /*
    printf("free ");
    */
    dump_node(node_info_last, false);
    
    
    node_info_last = node_info_last->parent;
    node_info_free(last_node_info);
}

void node_info_release_all() {
    while (node_info_last != NULL) {
        node_info_release();
    }
}

int on_file_item(const char* fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    char* name = NULL; 

    while ( (node_info_last != NULL) && (! is_sub_path(fpath, node_info_last->path)) ) {
        node_info_release();
    }

    lstat(fpath, stat_buf);

    if (node_info_last != NULL) {
        if (is_sub_path(fpath, node_info_last->path)) {
            name = get_path_name(node_info_last->path, fpath);
        } else {
            name = strdup(fpath);
        }
    } else {
        name = strdup(fpath);
    }
    
    struct node_info* node_info = node_info_create_from_parent(node_info_last == NULL ? NULL : node_info_last, fpath, name);

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

    if (node_info_root == NULL) {
        node_info_root = node_info;
    }
    node_info_last = node_info;

    return 0;
}


int main(int argc, char** argv) {
    char* path;
    if (argc < 1) {
        path = ".";
    } else {
        path = argv[1];
    }
    nftw(path, on_file_item, 200, 0);
    node_info_release_all();
    // dump_node(node_info_root, true);
    return 0;
}



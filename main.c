#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <stdbool.h>
#include "node_path.h"

struct stat st_stat_buf;
struct node_path* node_path_root;
struct node_path* node_path_last;

#define stat_buf (&st_stat_buf)

bool is_sub_path(const char* value, const char* pattern) {
    size_t value_len = strlen(value);
    size_t pattern_len = strlen(pattern);

    if (value_len < pattern_len+1) {
        return false;
    }

    if (strncmp(value, pattern, pattern_len) == 0) {
        if (value[pattern_len] == '/') {
            return true;
        }
        return false;
    }
}

char* get_path_name(const char* parent, const char* path) {
    /*
    printf("    z : [%s]\n", parent);
    printf("    y : [%s]\n", path);
    */
    return strdup(path+strlen(parent)+1);
}

void dump_node_paths() {
    struct node_path* node_path = node_path_root;
    while (node_path != NULL) {
        printf("    node path : [%s] [%ld] [%ld]\n", node_path->path, node_path->node->size, node_path->node->occ_size);
        node_path = node_path->next;
    }
}

void dump_node(struct node_info* node_info_item, int width);
void dump_node(struct node_info* node_info_item, int width) {
    int index_width;
    for (index_width = 0; index_width<width; index_width++) {
        printf("  ");
    }
    printf("[%s] [%ld] [%ld]\n", node_info_item->name, node_info_item->size, node_info_item->occ_size);
    struct node_info *child = node_info_item->first_child;
    while (child != NULL) {
        dump_node(child, width+1);
        child = child->next;
    }
}

int on_file_item(const char* fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    char* name = NULL; 

    // printf("on_file_item : [%s]\n", fpath);
    // dump_node_paths();

    while ( (node_path_last != NULL) && (! is_sub_path(fpath, node_path_last->path)) ) {
        // printf("node_path_free ([%s] is not sub path of [%s])\n", node_path_last->path, fpath);
        node_path_free(&node_path_last);
    }

    lstat(fpath, stat_buf);

    if (node_path_last != NULL) {
        if (is_sub_path(fpath, node_path_last->path)) {
            name = get_path_name(node_path_last->path, fpath);
        } else {
            name = strdup(fpath);
        }
    } else {
        name = strdup(fpath);
    }
    
    // printf("name: [%s]\n", name);

    if (node_path_last == NULL) {
        // printf("node_path_last == NULL\n");
    }

    //if (S_ISDIR(stat_buf->st_mode)) {
        struct node_path* node_path = NULL;
        struct node_info* node_info = node_info_create_from_parent(node_path_last == NULL ? NULL : node_path_last->node, fpath, name);
        node_path = node_path_create(fpath, node_info);

        size_type size = stat_buf->st_size;
        size_type occ_size = stat_buf->st_blksize*stat_buf->st_blocks/8;

        // printf("dir  : [%s] [%s] [%ld] [%ld]\n", fpath, name, size, occ_size);
        {
            struct node_info* current_node_info = node_info;
            while (current_node_info != NULL) {
                current_node_info->size += size;
                current_node_info->occ_size += occ_size;
                current_node_info = current_node_info->parent;
            }
        }

        if (node_path_root == NULL) {
            node_path_root = node_path;
            node_path_last = node_path;
        } else {
            node_path->prev = node_path_last;
            node_path_last->next = node_path;
            node_path_last = node_path;
        }
    //} else {
        // printf("file : [%s] [%s] [%ld]\n", fpath, name, stat_buf->st_size);
    //}

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
    dump_node(node_path_root->node,0);
    return 0;
}



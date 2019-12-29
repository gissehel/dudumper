#define _XOPEN_SOURCE 500
#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "mem_utils.h"
#include "node_info.h"
#include "node_info_consumer.h"
#include "node_parser.h"


struct node_parser {
    struct node_info_consumer* node_info_consumer;
    long depth;
};

struct node_parser_data {
    struct node_info* node_info_root;
    struct node_info* node_info_last;
    const struct node_parser* node_parser;
    bool consider_dot_dir_as_file;
};

static struct node_parser_data* node_parser_data;

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
    return MEM_STRDUP(path+strlen(parent)+1);
}

void dump_node_paths() {
    struct node_info* node_info = node_parser_data->node_info_last;
    while (node_info != NULL) {
        MEM_STRUCT_PTR_DEF_STRUCT(node_parser, node_parser_data->node_parser, node_info_consumer, node_info_consumer);
        if (node_info_consumer != NULL) {
            NODE_INFO_CONSUMER_CONSUME_NODE(node_info_consumer, node_info);
        }
        node_info = node_info->parent;
    }
}

void dump_node(struct node_info* node_info, bool recurse) {
    if (node_parser_data->node_parser->depth == 0 || node_parser_data->node_parser->depth >= node_info->depth) {
        MEM_STRUCT_PTR_DEF_STRUCT(node_parser, node_parser_data->node_parser, node_info_consumer, node_info_consumer);
        if (node_info_consumer != NULL) {
            if (node_info->should_dump) {
                NODE_INFO_CONSUMER_CONSUME_NODE(node_info_consumer, node_info);
            }
        }
        if (recurse) {
            struct node_info *child = node_info->first_child;
            while (child != NULL) {
                dump_node(child, recurse);
                child = child->next;
            }
        }
    }
}

void node_info_release() {
    struct node_info* last_node_info = node_parser_data->node_info_last;

    dump_node(node_parser_data->node_info_last, false);
    
    node_parser_data->node_info_last = node_parser_data->node_info_last->parent;
    node_info_free(last_node_info);
}

void node_info_release_all() {
    while (node_parser_data->node_info_last != NULL) {
        node_info_release();
    }
}

int on_file_item(const char* fpath, const struct stat *sb, int typeflag, struct FTW* ftwbuf) {
    UNUSED(sb);
    UNUSED(typeflag);
    UNUSED(ftwbuf);
    char* name = NULL; 
    struct stat st_stat_buf;

    while ( (node_parser_data->node_info_last != NULL) && (! is_sub_path(fpath, node_parser_data->node_info_last->path)) ) {
        node_info_release();
    }

    lstat(fpath, &st_stat_buf);

    if (node_parser_data->node_info_last != NULL) {
        if (is_sub_path(fpath, node_parser_data->node_info_last->path)) {
            name = get_path_name(node_parser_data->node_info_last->path, fpath);
        } else {
            name = MEM_STRDUP(fpath);
        }
    } else {
        name = MEM_STRDUP(fpath);
    }
    
    struct node_info* node_info = node_info_create_from_parent(node_parser_data->node_info_last == NULL ? NULL : node_parser_data->node_info_last, fpath, name);

    node_info->is_dir = S_ISDIR(st_stat_buf.st_mode);
    uint64_t size = (uint64_t)st_stat_buf.st_size;
    uint64_t occ_size = (uint64_t)(512)*(uint64_t)(st_stat_buf.st_blocks);

    if (node_info->name && node_info->name[0] == '.' && node_parser_data->consider_dot_dir_as_file) {
        node_info->children_should_dump = false;
    }

    if (node_info->parent && (! node_info->parent->children_should_dump)) {
        node_info->should_dump = false;
        node_info->children_should_dump = false;
    }

    {
        struct node_info* current_node_info = node_info;
        while (current_node_info != NULL) {
            current_node_info->size += size;
            current_node_info->occ_size += occ_size;
            current_node_info = current_node_info->parent;
        }
    }

    if (node_parser_data->node_info_root == NULL) {
        node_parser_data->node_info_root = node_info;
    }
    node_parser_data->node_info_last = node_info;

    MEM_FREE(name);

    return 0;
}

struct node_parser_data* node_parser_data_create(const struct node_parser* node_parser, bool consider_dot_dir_as_file) {
    MEM_ALLOC_STRUCT_DEF(node_parser_data);
    node_parser_data->node_info_root = NULL;
    node_parser_data->node_info_last = NULL;
    node_parser_data->consider_dot_dir_as_file = consider_dot_dir_as_file;
    node_parser_data->node_parser = node_parser;
    return node_parser_data;
}

void node_parser_data_free(struct node_parser_data* node_parser_data) {
    MEM_FREE(node_parser_data);
}

void node_parser_start(const struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser, node_parser, node_info_consumer, node_info_consumer);
    if (node_info_consumer != NULL) {
        NODE_INFO_CONSUMER_START(node_info_consumer);
    }
}

void node_parser_parse(struct node_parser* node_parser, const char* path, bool consider_dot_dir_as_file) {
    node_parser_data = node_parser_data_create(node_parser, consider_dot_dir_as_file);
    node_parser_start(node_parser_data->node_parser);
    nftw(path, on_file_item, 200, FTW_PHYS);
    node_info_release_all();
    node_parser_stop(node_parser_data->node_parser);
    node_parser_data_free(node_parser_data);
    node_parser_data = NULL;
}

void node_parser_stop(const struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser, node_parser, node_info_consumer, node_info_consumer);
    if (node_info_consumer != NULL) {
        NODE_INFO_CONSUMER_STOP(node_info_consumer);
    }
}

struct node_parser* node_parser_create(struct node_info_consumer* node_info_consumer, long depth) {
    MEM_ALLOC_STRUCT_DEF(node_parser);
    node_parser->node_info_consumer = node_info_consumer;
    node_parser->depth = depth;
    return node_parser;
}

void node_parser_free(struct node_parser* node_parser) {
    MEM_STRUCT_PTR_DEF_STRUCT(node_parser, node_parser, node_info_consumer, node_info_consumer);
    if (node_info_consumer != NULL) {
        /*
         * NODE_INFO_CONSUMER_DISPOSE(node_info_consumer);
         */
        MEM_STRUCT_PTR(node_parser, node_parser, node_info_consumer) = NULL;
    }
    MEM_FREE(node_parser);
}


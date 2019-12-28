#ifndef __NODE_INFO_H
#define __NODE_INFO_H
#include <stdbool.h>
#include <inttypes.h>

struct node_info {
    struct node_info* parent;
    struct node_info* first_child;
    struct node_info* last_child;
    struct node_info* prev;
    struct node_info* next;

    char* path;
    char* name;
    uint64_t size;
    uint64_t occ_size;
    long depth;
    long id;
    bool is_dir;
    bool should_dump;
    bool children_should_dump;
};

struct node_info* node_info_create();
struct node_info* node_info_create_from_parent(struct node_info* parent, const char* path, const char* name);
void node_info_free(struct node_info* node_info);

#endif /* __NODE_INFO_H */


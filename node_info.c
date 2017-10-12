#include <stdlib.h>
#include "node_info.h"

struct node_info* node_info_create() {
    struct node_info* node_info;
    node_info = malloc(sizeof(struct node_info));
    node_info->parent = NULL;
    node_info->first_child = NULL;
    node_info->last_child = NULL;
    node_info->prev = NULL;
    node_info->next = NULL;
    node_info->path = NULL;
    node_info->name = NULL;
    node_info->size = 0;
    node_info->occ_size = 0;
    return node_info;
}

struct node_info* node_info_create_from_parent(struct node_info* parent, const char* path, char* name) {
    struct node_info* node_info;
    node_info = node_info_create();
    node_info->parent = parent;
    node_info->path = strdup(path);
    node_info->name = name;

    if (parent != NULL) {
        if (parent->last_child == NULL) {
            parent->first_child = node_info;
            parent->last_child = node_info;
        } else {
            parent->last_child->next = node_info;
            node_info->prev = parent->last_child;
            parent->last_child = node_info;
        }
    }
    return node_info;
}

void node_info_free(struct node_info* node_info) {
    if (node_info == NULL) {
        return;
    }
    if (node_info->name != NULL) {
        free(node_info->name);
    }
    free(node_info);

}

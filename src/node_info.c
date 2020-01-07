#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "node_info.h"
#include "mem_utils.h"

long next_node_id = 1;

struct node_info* node_info_create() {
    MEM_ALLOC_STRUCT_DEF(node_info);
    node_info->parent = NULL;
    node_info->first_child = NULL;
    node_info->last_child = NULL;
    node_info->prev = NULL;
    node_info->next = NULL;
    node_info->path = NULL;
    node_info->name = NULL;
    node_info->size = 0;
    node_info->occ_size = 0;
    node_info->depth = 0;
    node_info->id = next_node_id;
    node_info->is_dir = false;
    node_info->should_dump = true;
    node_info->children_should_dump = true;
    next_node_id++;
    return node_info;
}

struct node_info* node_info_create_from_parent(struct node_info* parent, const char* path, const char* name) {
    struct node_info* node_info = node_info_create();
    node_info->parent = parent;
    node_info->path = MEM_STRDUP(path);
    node_info->name = MEM_STRDUP(name);
    
    if (parent != NULL) {
        if (parent->last_child == NULL) {
            parent->first_child = node_info;
            parent->last_child = node_info;
        } else {
            parent->last_child->next = node_info;
            node_info->prev = parent->last_child;
            parent->last_child = node_info;
        }
        node_info->depth = parent->depth+1;
    }

    return node_info;
}

void node_info_free(struct node_info* node_info) {
    if (node_info == NULL) {
        return;
    }
    if (node_info->first_child != NULL) {
        struct node_info* current = node_info->first_child;
        while (current != NULL) {
            if (current->parent == node_info) {
                current->parent = NULL;
                printf("-- making orphan [%s][%s]\n", current->name, current->path);
            }
            current = current->next;
        }
        node_info->first_child = NULL;
    }
    if (node_info->last_child != NULL) {
        struct node_info* current = node_info->last_child;
        while (current != NULL) {
            if (current->parent == node_info) {
                current->parent = NULL;
                printf("-- making orphan [%s][%s]\n", current->name, current->path);
            }
            current = current->next;
        }
        node_info->last_child = NULL;
    }
    struct node_info* prev = node_info->prev;
    struct node_info* next = node_info->prev;
    if (prev != NULL) {
        prev->next = next;
        node_info->prev = NULL;
    }
    if (next != NULL) {
        next->prev = prev;
        node_info->next = NULL;
    }
    if (node_info->parent != NULL) {
        if (node_info->parent->first_child == node_info) {
            node_info->parent->first_child = next;
        }
        if (node_info->parent->last_child == node_info) {
            node_info->parent->last_child = prev;
        }
    }
    MEM_SET_NULL_AND_FREE(node_info->path);
    MEM_SET_NULL_AND_FREE(node_info->name);
    MEM_FREE(node_info);
}


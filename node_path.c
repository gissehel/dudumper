#include <stdlib.h>
#include "node_path.h"

struct node_path* node_path_create(const char* path, struct node_info* node_info) {
    struct node_path* node_path;
    node_path = malloc(sizeof(struct node_path));
    node_path->path = strdup(path);
    node_path->node = node_info;
    node_path->prev = NULL;
    node_path->next = NULL;
    return node_path;
}

void node_path_free(struct node_path** p_node_path) {
    struct node_path* node_path;

    if (p_node_path == NULL || *p_node_path == NULL) {
        return;
    }
    node_path = *p_node_path;
    if (node_path->prev != NULL) {
        node_path->prev->next = NULL;
        *p_node_path = node_path->prev;
        node_path->prev = NULL;
    } else {
        *p_node_path = NULL;
    }
    free(node_path->path);
    free(node_path);

}


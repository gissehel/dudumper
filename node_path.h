#include "node_info.h"

struct node_path {
    char* path;
    struct node_info* node;
    struct node_path* prev;
    struct node_path* next;
};

struct node_path* node_path_create(const char* path, struct node_info* node_info);

void node_path_free(struct node_path** p_node_path);



typedef long size_type;

struct node_info {
    struct node_info* parent;
    struct node_info* first_child;
    struct node_info* last_child;
    struct node_info* prev;
    struct node_info* next;

    char* path;
    char* name;
    size_type size;
    size_type occ_size;
};

struct node_info* node_info_create();
struct node_info* node_info_create_from_parent(struct node_info* parent, const char* path, char* name);
void node_info_free(struct node_info* node_info);




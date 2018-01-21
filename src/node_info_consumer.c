#include "mem_utils.h"
#include "node_info_consumer.h"

void node_info_consumer_free(struct node_info_consumer* node_info_consumer) {
    if (node_info_consumer != NULL) {
        NODE_INFO_CONSUMER_DISPOSE(node_info_consumer);
        MEM_FREE(node_info_consumer);
    }
}


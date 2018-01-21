#ifndef __NODE_INFO_CONSUMER_H
#define __NODE_INFO_CONSUMER_H

struct node_info;

struct node_info_consumer {
    void (*on_start)(const struct node_info_consumer* node_info_consumer);
    void (*on_stop)(const struct node_info_consumer* node_info_consumer);
    void (*on_consume_node)(const struct node_info_consumer* node_info_consumer, struct node_info* node_info);
    void (*on_dispose)(struct node_info_consumer* node_info_consumer);
};

#define NODE_INFO_CONSUMER_START(node_info_consumer) (node_info_consumer->on_start(node_info_consumer))
#define NODE_INFO_CONSUMER_STOP(node_info_consumer) (node_info_consumer->on_stop(node_info_consumer))
#define NODE_INFO_CONSUMER_CONSUME_NODE(node_info_consumer, node_info) (node_info_consumer->on_consume_node(node_info_consumer, node_info))
#define NODE_INFO_CONSUMER_DISPOSE(node_info_consumer) (node_info_consumer->on_dispose(node_info_consumer))
void node_info_consumer_free(struct node_info_consumer*);

#endif /* __NODE_INFO_CONSUMER_H */


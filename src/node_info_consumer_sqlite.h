#ifndef __NODE_INFO_CONSUMER_SQLITE_H
#define __NODE_INFO_CONSUMER_SQLITE_H

struct node_info_consumer;
struct string_dumper;
struct node_info_consumer* node_info_consumer_sqlite_create(struct string_dumper* string_dumper);

#endif /* __NODE_INFO_CONSUMER_SQLITE_H */

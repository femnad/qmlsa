#include <stdlib.h>

#include "quic_packet.h"

typedef enum {HANDLE_RESET_PACKET, HANDLE_VERSION_NEGOTATION, HANDLE_REGULAR_PACKET,
              HANDLE_REGULAR_PACKET_WITH_VERSION, UNHANDLED} quic_action;

void print_quic_packet(quic_packet *);
quic_action handle_packet(quic_packet *);

void *
get_sub_range(void *, int, int);

char *
serialize_quic_packet(quic_packet *);

quic_version_packet *
get_quic_version_packet_from_buffer(Bytes, size_t);

void
build_quic_packet_buffer(Bytes, unsigned int, unsigned long, unsigned int,
                         unsigned int, Bytes);

long
network_ordered_bytes_to_long(Bytes);

Bytes
connection_id_to_network_ordered_bytes(long);

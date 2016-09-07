#include <stdlib.h>

typedef enum {HANDLE_RESET_PACKET, HANDLE_VERSION_NEGOTATION, HANDLE_REGULAR_PACKET,
              HANDLE_REGULAR_PACKET_WITH_VERSION, UNHANDLED} quic_action;
typedef unsigned char * Bytes;

void print_quic_packet(quic_packet *);
quic_action handle_packet(quic_packet *);

void *
get_sub_range(void *, int, int);

char *
serialize_quic_packet(quic_packet *);

quic_packet_buffer *
get_quic_packet_from_buffer(Bytes, size_t);

#include <stdio.h>

#include "quic_packet.h"

void print_quic_packet(struct quic_packet *__qp) {
    printf("%s\n", "Public header:");
    printf("Public flags: %d\n", __qp->public_header.public_flags);
    printf("Connection ID: %d\n", __qp->public_header.connection_id);
    printf("QUIC version: %s\n", __qp->public_header.quic_version);
    printf("Packet number: %d\n", __qp->public_header.packet_number);
}

int should_reset(struct quic_packet *__qp) {
    return __qp->public_header.public_flags & PUBLIC_FLAG_RESET;
}

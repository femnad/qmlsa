#include <stdio.h>

#include "quic_packet.h"
#include "quic_util.h"

void print_quic_packet(quic_packet *__qp) {
    printf("%s\n", "Public header:");
    printf("Public flags: %d\n", __qp->public_flags);
    printf("Connection ID: %ld\n", __qp->connection_id);
    printf("QUIC version: %u\n", __qp->quic_version);
    printf("Packet number: %ld\n", __qp->packet_number);
}

quic_action handle_packet(quic_packet *__qp) {
    if (__qp->public_flags & PUBLIC_FLAG_RESET) {
        return HANDLE_RESET_PACKET;
    }
    return UNHANDLED;
}

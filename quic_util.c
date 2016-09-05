#include <stdio.h>

#include "quic_packet.h"
#include "quic_util.h"

void print_quic_packet(quic_packet *__qp) {
    printf("%s\n", "Public header:");
    printf("Public flags: %d\n", __qp->public_header.public_flags);
    printf("Connection ID: %ld\n", __qp->public_header.connection_id);
    printf("QUIC version: %s\n", __qp->public_header.quic_version);
    printf("Packet number: %d\n", __qp->public_header.packet_number);
}

quic_action handle_packet(quic_packet *__qp) {
    if (__qp->public_header.public_flags & PUBLIC_FLAG_RESET) {
        return HANDLE_RESET_PACKET;
    }
    return UNHANDLED;
}

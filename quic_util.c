#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quic_packet.h"
#include "quic_util.h"

#define BUFFER_SIZE 64

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

char *
get_sub_range(char *buffer, int start, int end) {
    int sub_buffer_size = end - start + 1;
    char *sub_range = malloc(sub_buffer_size * sizeof(char));
    memcpy(sub_range, buffer+1, sub_buffer_size);
    return sub_range;
}

char *
serialize_quic_packet(quic_packet *__qp) {
    char *buffer = malloc(BUFFER_SIZE * sizeof(char));
    memcpy(buffer, &(__qp->public_flags), 1);
    memcpy(buffer+1, &(__qp->connection_id), 4);
    memcpy(buffer+5, &(__qp->quic_version), 4);
    memcpy(buffer+9, &(__qp->packet_number), 4);
    char *payload = "3e8f101cdff6bab"; // nonsensical payload
    size_t payload_size = strlen(payload);
    memcpy(buffer+13, &payload, payload_size);
    memcpy(buffer+13+payload_size, "/0", 1);
    return buffer;
}

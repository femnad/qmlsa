#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quic_packet.h"
#include "quic_util.h"

#define BUFFER_SIZE 64
#define CONNECTION_ID_SIZE 8
#define QUIC_VERSION_SIZE 4
#define PAYLOAD_SIZE 18
#define PUBLIC_FLAGS_SIZE 1
#define SEQUENCE_SIZE 1

void *
get_sub_range(void *buffer, int start, int end) {
    size_t sub_buffer_size = end - start + 1;
    void *head = buffer + start;
    char *sub_range = malloc(sub_buffer_size * sizeof(*(head)));
    memcpy(sub_range, head, sub_buffer_size);
    return sub_range;
}

char *
serialize_quic_packet(quic_packet *__qp) {
    char *buffer = malloc(BUFFER_SIZE * sizeof(char));
    memcpy(buffer, &(__qp->public_flags), 1);
    memcpy(buffer+1, &(__qp->connection_id), 4);
    memcpy(buffer+5, &(__qp->quic_version), 4);
    memcpy(buffer+9, &(__qp->sequence), 4);
    char *payload = "3e8f101cdff6bab"; // nonsensical payload
    size_t payload_size = strlen(payload);
    memcpy(buffer+13, &payload, payload_size);
    memcpy(buffer+13+payload_size, "/0", 1);
    return buffer;
}

quic_packet_buffer *
get_quic_packet_from_buffer(Bytes buffer, size_t packet_size) {
    quic_packet_buffer *__qp = malloc(sizeof(quic_packet_buffer));
    Bytes public_flags = get_sub_range(buffer, 0, 0);
    Bytes connection_id = get_sub_range(buffer, 1, 8);
    Bytes quic_version = get_sub_range(buffer, 9, 12);
    Bytes sequence = get_sub_range(buffer, 13, 13);
    Bytes payload = get_sub_range(buffer, 14, packet_size);
    __qp->public_flags = public_flags;
    __qp->connection_id = connection_id;
    __qp->quic_version = quic_version;
    __qp->sequence = sequence;
    __qp->payload = payload;
    return __qp;
}

void
build_quic_packet_buffer(Bytes packet_buffer, Bytes flags, Bytes connection_id,
                       Bytes quic_version, Bytes sequence, Bytes payload) {
    Bytes p = packet_buffer;
    int cursor = 0;
    for (int i = 0; i < PUBLIC_FLAGS_SIZE; i++) {
        *(p + cursor) = *(flags + i);
        cursor++;
    }
    for (int i = 0; i < CONNECTION_ID_SIZE; i++) {
        *(p + cursor) = *(connection_id + i);
        cursor++;
    }
    for (int i = 0; i < QUIC_VERSION_SIZE; i++) {
        *(p + cursor) = *(quic_version + i);
        cursor++;
    }
    for (int i = 0; i < SEQUENCE_SIZE; i++) {
        *(p + cursor) = *(sequence + i);
        cursor++;
    }
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        *(p + cursor) = *(payload + i);
        cursor++;
    }
}

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

quic_version_packet *
get_quic_version_packet_from_buffer(Bytes buffer, size_t packet_size) {
    quic_version_packet *__qvp = malloc(sizeof(quic_version_packet));
    __qvp->public_flags = get_sub_range(buffer, 0, 0);
    __qvp->connection_id = get_sub_range(buffer, PUBLIC_FLAGS_SIZE,
                                         CONNECTION_ID_SIZE);
    int version_index = 0;
    int version_start = PUBLIC_FLAGS_SIZE + CONNECTION_ID_SIZE;
    int number_of_versions = (packet_size - version_start) / QUIC_VERSION_SIZE;
    __qvp->versions = malloc(sizeof(char *) * number_of_versions);
    for (int i = version_start; i+QUIC_VERSION_SIZE <= (int) packet_size;
         i+=QUIC_VERSION_SIZE) {
        char * current_version = *(__qvp->versions + version_index);
        current_version = get_sub_range(buffer, i, i+QUIC_VERSION_SIZE-1);
        *(__qvp->versions + version_index) = current_version;
        version_index++;
    }
    __qvp->number_of_supported_versions = version_index;
    return __qvp;
}

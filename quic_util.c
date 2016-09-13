#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "quic_packet.h"
#include "quic_util.h"

#define BUFFER_SIZE 64
#define CONNECTION_ID_SIZE 8
#define HARDCODED_PACKET_SIZE 19
#define HEXADECIMAL_BASE 16
#define LOG_2_OF_16 4
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

unsigned long get_random_connection_id() {
    srand48(time(NULL));
    unsigned long two_to_63 = pow(2, 63);
    double random_multiplier = drand48() + 1;
    return (unsigned long) (two_to_63 * random_multiplier);
}

char *
serialize_quic_packet(quic_packet __qp) {
    char *buffer = malloc(HARDCODED_PACKET_SIZE * sizeof(char));
    memcpy(buffer, "\r", 1);
    Bytes connection_id = connection_id_to_network_ordered_bytes(__qp.connection_id);
    memcpy(buffer+1, connection_id, 8);
    memcpy(buffer+9, __qp.quic_version, 4);
    memcpy(buffer+13, "\001", 1);
    memcpy(buffer+14, "CHLO", 4);
    memcpy(buffer+19, "\0", 1);
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
    for (int i = version_start; i + QUIC_VERSION_SIZE <= (int) packet_size;
         i += QUIC_VERSION_SIZE) {
        *(__qvp->versions + version_index) = get_sub_range(buffer, i, i + QUIC_VERSION_SIZE - 1);
        version_index++;
    }
    __qvp->number_of_supported_versions = version_index;
    return __qvp;
}

unsigned long
network_ordered_bytes_to_long(Bytes buffer) {
    long total = 0;
    size_t buffer_length = strlen((const char *) buffer);
    for (unsigned long i = 0; i < buffer_length; i++) {
        total += (int) *(buffer + i) * (long) pow(HEXADECIMAL_BASE, i * 2);
    }
    return total;
}

Bytes
connection_id_to_network_ordered_bytes(unsigned long number) {
    Bytes buffer = malloc(sizeof(char) * CONNECTION_ID_SIZE);
    // Start allocating from the end
    int power = 14;
    unsigned char host_ordered_bytes[CONNECTION_ID_SIZE];
    int cursor = 0;
    while (number > 1) {
        int digit = number >> (power * LOG_2_OF_16);
        host_ordered_bytes[cursor++] = (unsigned char) digit;
        number -= digit * (long) pow(HEXADECIMAL_BASE, power);
        power -= 2;
    }
    for (int i = 0; i < CONNECTION_ID_SIZE; i++) {
        *(buffer + i) = host_ordered_bytes[CONNECTION_ID_SIZE - 1 - i];
    }
    return buffer;
}

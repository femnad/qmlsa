#include <stdint.h>

#ifndef _QUIC_PACKET_H
#define _QUIC_PACKET_H 1

#define PUBLIC_FLAG_VERSION 0x01
#define PUBLIC_FLAG_RESET 0x02
#define PUBLIC_FLAG_DIVERSIFICATION_NONCE 0x04
#define PUBLIC_FLAG_FULL_CID_PRESENT 0x08
#define RESERVED_PUBLIC_FLAG_MULTIPATH 0x40
#define UNUSED_PUBLIC_FLAG 0x80

typedef unsigned char * Bytes;

typedef struct _quic_packet {
    unsigned int public_flags;
    unsigned long int connection_id;
    unsigned int quic_version;
    unsigned int sequence;
    unsigned char *payload;
} quic_packet;

typedef struct _quic_version_packet {
    Bytes public_flags;
    Bytes connection_id;
    char **versions;
    int number_of_supported_versions;
} quic_version_packet;

typedef struct _quic_packet_buffer {
    unsigned char *public_flags;
    unsigned char *connection_id;
    unsigned char *quic_version;
    unsigned char *sequence;
    unsigned char *payload;
} quic_packet_buffer;

#endif // _QUIC_PACKET_H

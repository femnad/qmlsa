#include <stdint.h>

#define PUBLIC_FLAG_VERSION 0x01
#define PUBLIC_FLAG_RESET 0x02
#define PUBLIC_FLAG_DIVERSIFICATION_NONCE 0x04
#define PUBLIC_FLAG_FULL_CID_PRESENT 0x08
#define RESERVED_PUBLIC_FLAG_MULTIPATH 0x40
#define UNUSED_PUBLIC_FLAG 0x80

typedef struct _quic_packet {
    unsigned int public_flags;
    unsigned long int connection_id;
    unsigned int quic_version;
    unsigned long int packet_number;
} quic_packet;

#define PUBLIC_FLAG_VERSION 0x01
#define PUBLIC_FLAG_RESET 0x02

struct quic_packet {
    int public_flags;
    unsigned int connection_id;
};

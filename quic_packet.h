#define PUBLIC_FLAG_VERSION 0x01
#define PUBLIC_FLAG_RESET 0x02
#define CONNECTION_ID_PRESENT 0x08

struct quic_public_packet_header {
    int public_flags;
    unsigned int connection_id;
    char quic_version[5];
    int packet_number;
};

struct quic_packet {
    struct quic_public_packet_header public_header;
};

typedef enum {HANDLE_RESET_PACKET, HANDLE_VERSION_NEGOTATION, HANDLE_REGULAR_PACKET,
              HANDLE_REGULAR_PACKET_WITH_VERSION, UNHANDLED} quic_action;

void print_quic_packet(quic_packet *);
quic_action handle_packet(quic_packet *);

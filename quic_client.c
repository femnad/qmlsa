#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "quic_packet.h"
#include "quic_util.h"

int main(int argc, char **argv) {
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in svaddr;
    struct quic_packet *qp_response = malloc(sizeof(struct quic_packet));
    struct quic_public_packet_header *response_header =
        malloc(sizeof(struct quic_public_packet_header));
    qp_response->public_header = *response_header;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(9876);
    inet_pton(AF_INET, "127.0.0.1", &svaddr.sin_addr);

    struct quic_packet *qp_request = malloc(sizeof(struct quic_packet));
    struct quic_public_packet_header *public_header =
        malloc(sizeof(struct quic_public_packet_header));
    public_header->public_flags = PUBLIC_FLAG_VERSION;
    public_header->packet_number = 1;
    srand(time(NULL));
    public_header->connection_id = rand();
    qp_request->public_header = *public_header;
    while (1) {
        int sent_bytes = sendto(cfd, qp_request, sizeof(struct quic_packet), 0,
                                (struct sockaddr *) &svaddr,
                                sizeof(struct sockaddr));
        printf("Sent %d bytes\n", sent_bytes);
        int received_bytes = recvfrom(cfd, qp_response,
                                      sizeof(struct quic_packet), 0, NULL, NULL);
        printf("Received %d bytes\n", received_bytes);
        print_quic_packet(qp_response);
        if (should_reset(qp_response)) {
            printf("Resetting connection per public flag");
        }
        break;
    }
    free(qp_request);
    free(qp_response);
}

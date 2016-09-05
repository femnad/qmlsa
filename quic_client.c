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

#define INITIAL_VERSION "Q034"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: quic_client <host> <port>\n");
        exit(1);
    }
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in svaddr;
    quic_packet *qp_response = malloc(sizeof(quic_packet));
    quic_public_packet_header *response_header =
        malloc(sizeof(quic_public_packet_header));
    qp_response->public_header = *response_header;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &svaddr.sin_addr);

    quic_packet *qp_request = malloc(sizeof(quic_packet));
    quic_public_packet_header *public_header =
        malloc(sizeof(quic_public_packet_header));
    public_header->public_flags = PUBLIC_FLAG_VERSION;
    public_header->packet_number = 1;
    srand(time(NULL));
    public_header->connection_id = rand();
    strcpy(public_header->quic_version, INITIAL_VERSION);
    qp_request->public_header = *public_header;
    qp_request->sequence_number = 1;
    int reset = 0;
    while (1) {
        int sent_bytes = sendto(cfd, qp_request, sizeof(quic_packet), 0,
                                (struct sockaddr *) &svaddr,
                                sizeof(struct sockaddr));
        printf("Sent %d bytes\n", sent_bytes);
        int received_bytes = recvfrom(cfd, qp_response,
                                      sizeof(quic_packet), 0, NULL, NULL);
        printf("Received %d bytes\n", received_bytes);
        print_quic_packet(qp_response);
        quic_action response_action = handle_packet(qp_response);

        switch (response_action) {
        case HANDLE_RESET_PACKET:
            reset = 1;
            break;
        case UNHANDLED:
            printf("Unhandled package\n");
        default:
            printf("You shouldn't be seeing this\n");
        }

        if (reset) {
            break;
        }
    }
    free(qp_request);
    free(qp_response);
}

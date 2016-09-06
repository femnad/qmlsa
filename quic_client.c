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

#define INITIAL_VERSION 0x51303336

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: quic_client <host> <port>\n");
        exit(1);
    }
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in svaddr;
    quic_packet *qp_response = malloc(sizeof(quic_packet));
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &svaddr.sin_addr);

    quic_packet *qp_request = malloc(sizeof(quic_packet));
    qp_request->public_flags = 0x0d;
    srand(time(NULL));
    int connection_id = rand();
    qp_request->connection_id = connection_id;
    printf("Connection ID is %d\n", connection_id);
    qp_request->quic_version = htonl(INITIAL_VERSION);
    qp_request->packet_number = 1;
    char *buffer = malloc(64 * sizeof(char));
    memcpy(buffer, &(qp_request->public_flags), 1);
    memcpy(buffer+1, &(qp_request->connection_id), 8);
    memcpy(buffer+9, &(qp_request->quic_version), 4);
    memcpy(buffer+13, &(qp_request->packet_number), 4);
    long int payload = 0x3e8f101cdff6bab; // nonsensical payload
    memcpy(buffer+17, &payload, sizeof(payload));
    int reset = 0;
    while (1) {
        size_t buffer_size = sizeof(char) * 17 + sizeof(payload);
        int sent_bytes = sendto(cfd, buffer, buffer_size, 0,
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

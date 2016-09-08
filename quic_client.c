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
#define MAGIC_NUMBER 17

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
    qp_request->public_flags = PUBLIC_FLAG_VERSION | PUBLIC_FLAG_FULL_CID_PRESENT;
    srand(time(NULL));
    int connection_id = rand();
    qp_request->connection_id = connection_id;
    qp_request->quic_version = htonl(INITIAL_VERSION);
    qp_request->sequence = 1;
    char *buffer = serialize_quic_packet(qp_request);
    unsigned char *receive_buffer = malloc(sizeof(char) * 64);
    while (1) {
        sendto(cfd, buffer, MAGIC_NUMBER, 0, (struct sockaddr *) &svaddr,
               sizeof(struct sockaddr));
        int received_bytes = recvfrom(cfd, receive_buffer, 64, 0, NULL, NULL);
        quic_version_packet *server_response = get_quic_version_packet_from_buffer(
                                                                   receive_buffer,
                                                                   received_bytes);
        long connection_id = network_ordered_bytes_to_long(server_response->connection_id);
        printf("Connection ID is: %ld\n", connection_id);
        printf("Supported versions:\n");
        for (int i = 0; i < server_response->number_of_supported_versions; i++) {
            printf("%s\n", *(server_response->versions + i));
        }
        break;
    }
    free(qp_request);
    free(qp_response);
}

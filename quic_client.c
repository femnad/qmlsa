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
    char *buffer = serialize_quic_packet(qp_request);
    unsigned char *receive_buffer = malloc(sizeof(char) * 64);
    while (1) {
        int sent_bytes = sendto(cfd, buffer, strlen(buffer), 0, (struct sockaddr *) &svaddr,
               sizeof(struct sockaddr));
        if (sent_bytes == -1) {
            perror("Sendto error");
            exit(1);
        }
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

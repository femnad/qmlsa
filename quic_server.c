#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "quic_packet.h"

int main(int argc, char **argv) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_NUMERICSERV;

    struct addrinfo *result, *rp;
    getaddrinfo("127.0.0.1", "9876", &hints, &result);
    int sfd, num_read;
    socklen_t len;
    struct sockaddr_storage claddr;

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
        close(sfd);
    }

    struct quic_packet *client_request = malloc(sizeof(struct quic_packet));
    for (;;) {
        num_read = recvfrom(sfd, client_request, sizeof(struct quic_packet), 0,
                            (struct sockaddr *) &claddr, &len);
        printf("Read %d bytes\n", num_read);
        struct quic_packet *qp = malloc(sizeof(struct quic_packet));
        struct quic_public_packet_header *public_header =
            malloc(sizeof(struct quic_public_packet_header));
        public_header->public_flags = PUBLIC_FLAG_VERSION | PUBLIC_FLAG_RESET;
        strcpy(public_header->quic_version, "Q025");
        qp->public_header = *public_header;
        printf("Got public flag: %d\n", client_request->public_header.public_flags);
        printf("Connection id is %d\n", client_request->public_header.connection_id);
        int sent_bytes = sendto(sfd, qp, sizeof(struct quic_packet), 0,
                                (struct sockaddr *) &claddr, len);
        printf("Sent %d bytes\n", sent_bytes);
    }

}

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

int main(int argc, char **argv) {
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in svaddr;
    struct quic_packet *qp_response = malloc(sizeof(struct quic_packet));
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(9876);
    inet_pton(AF_INET, "127.0.0.1", &svaddr.sin_addr);

    struct quic_packet *qp_request = malloc(sizeof(struct quic_packet));
    qp_request->public_flags = PUBLIC_FLAG_VERSION;
    srand(time(NULL));
    qp_request->connection_id = rand();
    sendto(cfd, qp_request, sizeof(qp_request), 0, (struct sockaddr *) &svaddr,
           sizeof(struct sockaddr));
    int num_bytes = recvfrom(cfd, qp_response, sizeof(struct quic_packet), 0,
                             NULL, NULL);
    if (num_bytes == -1) {
        printf("recvfrom fail");
        exit(-1);
    }
    printf("Got public flag: %d\n", qp_response->public_flags);
}

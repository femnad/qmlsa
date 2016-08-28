all: quic_server quic_client

quic_server: quic_server.c quic_packet.h quic_util.h
	clang -Wall -g quic_util.c quic_server.c -o quic_server

quic_client: quic_client.c quic_packet.h quic_util.c
	clang -Wall -g quic_util.c quic_client.c -o quic_client

clean:
	rm -f quic_server
	rm -f quic_client

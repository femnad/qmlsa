all: quic_server quic_client

quic_server: quic_server.c
	clang quic_server.c -o quic_server

quic_client: quic_client.c
	clang quic_client.c -o quic_client

clean:
	rm -f quic_server
	rm -f quic_client

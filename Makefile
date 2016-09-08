CC=clang
CFLAGS=-Wall
DEBUG_FLAGS=$(CFLAGS) -g
HEADER_FILES=quic_packet.h quic_util.h
LD_FLAGS=-lm
UTIL_FILES=quic_util.c

all: quic_server quic_client

quic_server: quic_server.c $(HEADER_FILES)
	$(CC) $(CFLAGS) quic_server.c $(UTIL_FILES) -o quic_server

quic_client: quic_client.c $(HEADER_FILES)
	$(CC) $(CFLAGS) quic_client.c $(UTIL_FILES) -o quic_client $(LD_FLAGS)

clean:
	rm -f quic_server
	rm -f quic_client

debug_quic_server: quic_server.c $(HEADER_FILES)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) quic_server.c $(UTIL_FILES) -o quic_server

debug_quic_client: quic_client.c $(HEADER_FILES)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) quic_client.c $(UTIL_FILES) -o quic_client $(LD_FLAGS)

debug: debug_quic_server debug_quic_client

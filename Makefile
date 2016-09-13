CC=clang
CFLAGS=-Wall
HEADER_FILES=quic_packet.h quic_util.h
LD_FLAGS=-lm
UTIL_FILES=quic_util.c

all: quic_server quic_client

debug: CFLAGS += -g
debug: all

quic_server: quic_server.c $(HEADER_FILES) $(UTIL_FILES)
	$(CC) $(CFLAGS) quic_server.c $(UTIL_FILES) -o quic_server $(LD_FLAGS)

quic_client: quic_client.c $(HEADER_FILES) $(UTIL_FILES)
	$(CC) $(CFLAGS) quic_client.c $(UTIL_FILES) -o quic_client $(LD_FLAGS)

clean:
	rm -f quic_server
	rm -f quic_client

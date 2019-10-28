#include "pkt_decoder.h"
#include <stdio.h>

static void pkt_printer(void *ctx, size_t len, const uint8_t *data) {
    (void) ctx;
    printf("pkt (%zd bytes) -", len);
    for (size_t i = 0; i < len; i++) {
        printf(" %02x", data[i]);
    }
    printf("\n");
}

int main() {
    const uint8_t pkt1[] = {0x02, 0xFF, 0x03, 0x03, 0x02, 0x03, 0x05, 0x03};
    const uint8_t pkt2[] = {0x02, 0xFF, 0x10, 0x22, 0x45, 0x03};
    const uint8_t pkt3[] = {0x02, 0x07, 0x03, 0x10};
    pkt_decoder_t *decoder = pkt_decoder_create(pkt_printer, nullptr);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_write_bytes(decoder, sizeof(pkt2), pkt2);
    pkt_decoder_destroy(decoder);
    pkt_decoder_write_bytes(decoder, sizeof(pkt3), pkt3);
    return 0;
}
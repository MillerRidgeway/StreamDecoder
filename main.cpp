#include "pkt_decoder.h"
#include <stdio.h>
#include <vector>

static void pkt_printer(void *ctx, size_t len, const uint8_t *data) {
    (void) ctx;
    printf("pkt (%zd bytes) -", len);
    for (size_t i = 0; i < len; i++) {
        printf(" %02x", data[i]);
    }
    printf("\n");
}

int main() {
    //Testing vectors
    std::vector<const uint8_t*> packets;
    std::vector<int> sizes;

    //Test cases
    const uint8_t pkt0[] = {}; // Blank
    packets.push_back(pkt0);
    sizes.push_back(sizeof(pkt0));

    const uint8_t pkt1[] = {0x02, 0xFF, 0x03, 0x03, 0x02, 0x03, 0x05, 0x03}; // ff
    packets.push_back(pkt1);
    sizes.push_back(sizeof(pkt1));

    const uint8_t pkt2[] = {0x02, 0xFF, 0x10, 0x22, 0x45, 0x03}; // ff 02 45
    packets.push_back(pkt2);
    sizes.push_back(sizeof(pkt2));

    const uint8_t pkt3[] = {0x02, 0x07, 0x03, 0x10}; // 07 
    packets.push_back(pkt3);
    sizes.push_back(sizeof(pkt3));

    const uint8_t pkt4[] = {0x02, 0x10, 0x22, 0x03}; //02
    packets.push_back(pkt4);
    sizes.push_back(sizeof(pkt4));

    const uint8_t pkt5[] = {0x02, 0x10, 0x23, 0x03}; // 03
    packets.push_back(pkt5);
    sizes.push_back(sizeof(pkt5));

    const uint8_t pkt6[] = {0x02, 0x10, 0x30, 0x03};// 10
    packets.push_back(pkt6);
    sizes.push_back(sizeof(pkt6));

    const uint8_t pkt7[] = {0x02, 0x10, 0x30}; // Partial
    packets.push_back(pkt7);
    sizes.push_back(sizeof(pkt7));

    const uint8_t pkt8[] = {0x08, 0x03}; // 10 08
    packets.push_back(pkt8);
    sizes.push_back(sizeof(pkt8));

    const uint8_t pkt9[] = {0x02, 0x03}; // Blank
    packets.push_back(pkt9);
    sizes.push_back(sizeof(pkt9));

    uint8_t pkt10[520] = {}; // Packet > MAX_PACKET_LENGTH
    for(int i = 0; i < 520; i++){
        pkt10[i] = 0x01;
    }
    packets.push_back(pkt10);
    sizes.push_back(sizeof(pkt10));

    //Write test bytes
    pkt_decoder_t *decoder = pkt_decoder_create(pkt_printer, nullptr);
    for(int i = 0; i < packets.size(); i++){
         pkt_decoder_write_bytes(decoder, sizes.at(i), packets.at(i));
    }

    //Cleanup
    pkt_decoder_destroy(decoder);
    return 0;
}
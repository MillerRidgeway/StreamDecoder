//
// Created by Miller Ridgeway on 10/26/2019.
//
#ifndef STREAMDECODER_PKT_DECODER_H
#define STREAMDECODER_PKT_DECODER_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DECODED_PACKET_LEN (512)
typedef struct pkt_decoder pkt_decoder_t;
typedef void (*pkt_read_fn_t)(void *ctx, size_t len, const uint8_t *data);

// Constructor for a pkt_decoder
pkt_decoder_t *pkt_decoder_create(pkt_read_fn_t callback, void *callback_ctx);

// Destructor for a pkt_decoder
void pkt_decoder_destroy(pkt_decoder_t *decoder);

// Called on incoming, undecoded bytes to be translated into packets
void pkt_decoder_write_bytes(pkt_decoder_t *decoder, size_t len, const uint8_t *data);

#ifdef __cplusplus
}
#endif


#endif //STREAMDECODER_PKT_DECODER_H

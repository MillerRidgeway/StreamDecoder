//
// Created by Miller Ridgeway on 10/27/2019.
//

#include "pkt_decoder.h"
#include <string>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

struct pkt_decoder {
    pkt_read_fn_t callbackFunction;
    void *callbackContext;

    vector <uint8_t> immutableDataCopy, parsedStream;
    bool isComplete = false;
    bool escapeFlag = false;
    bool validStart = false;

    ~pkt_decoder() {}
};

pkt_decoder_t *pkt_decoder_create(pkt_read_fn_t callback, void *callback_ctx) {
    pkt_decoder *decoder = new pkt_decoder();
    decoder->callbackContext = callback_ctx;
    decoder->callbackFunction = callback;
    return decoder;
}

void pkt_decoder_destroy(pkt_decoder_t *decoder) {
    delete decoder;
    decoder = nullptr;
}

void pkt_decoder_write_bytes(pkt_decoder_t *decoder, size_t len, const uint8_t *data) {
    //Prelim checks (length, nulls, etc.)
    if (len > MAX_DECODED_PACKET_LEN) {
        cout << "ERROR: Packet length exceeded max value - " << MAX_DECODED_PACKET_LEN;
        return;
    }
    if (decoder == nullptr) {
        cout << "ERROR: Decoder cannot be null";
    }

    if (data == nullptr) {
        cout << "ERROR: Data cannot be null";
    }

    //Everything OK. Decode the stream.

    for (int i = 0; i < len; i++) {
        decoder->immutableDataCopy.push_back(data[i]);

        if (decoder->immutableDataCopy.at(i) == 0x03) { //ETX
            decoder->isComplete = true;
        } else if (decoder->immutableDataCopy.at(i) == 0x02) { //STX
            decoder->parsedStream.clear();
            decoder->validStart = true;
        } else if (decoder->immutableDataCopy.at(i) == 0x10) { //Found escape val
            decoder->escapeFlag = true;
        } else if (decoder->escapeFlag) { // Prev was escape (0x10)
            decoder->parsedStream.push_back(decoder->immutableDataCopy.at(i) - 0x20);
            decoder->escapeFlag = false;
        } else if (decoder->validStart) { // Add the value if we have seen proper STX
            decoder->parsedStream.push_back(decoder->immutableDataCopy.at(i));
        }

        //If we have found a valid packet stream print that packet and clear the parsing buffer
        if (decoder->isComplete) {
            if (!decoder->parsedStream.empty()) {
                uint8_t *newData = decoder->parsedStream.data();
                decoder->callbackFunction(decoder->callbackContext, decoder->parsedStream.size(), newData);
            }
            decoder->parsedStream.clear();
            decoder->isComplete = false;
            decoder->validStart = false;
        }
    }

    decoder->callbackContext;
}
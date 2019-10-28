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
    vector <uint8_t> immutableDataCopy, parsedStream;
    bool isComplete = false;
    bool escapeFlag = false;
    bool validStart = false;

    for (int i = 0; i < len; i++) {
        immutableDataCopy.push_back(data[i]);

        if (immutableDataCopy.at(i) == 0x03) { //ETX
            isComplete = true;
        } else if (immutableDataCopy.at(i) == 0x02) { //STX
            parsedStream.clear();
            validStart = true;
        } else if (immutableDataCopy.at(i) == 0x10) { //Found escape val
            escapeFlag = true;
        } else if (escapeFlag) { // Prev was escape (0x10)
            parsedStream.push_back(immutableDataCopy.at(i) - 0x20);
            escapeFlag = false;
        } else if (validStart) { // Add the value if we have seen proper STX
            parsedStream.push_back(immutableDataCopy.at(i));
        }

        //If we have found a valid packet stream print that packet and clear the parsing buffer
        if (isComplete) {
            if (!parsedStream.empty()) {
                uint8_t *newData = &parsedStream[0];
                decoder->callbackFunction(decoder->callbackContext, parsedStream.size(), newData);
            }
            parsedStream.clear();
            isComplete = false;
            validStart = false;
        }
    }

    decoder->callbackContext;
}
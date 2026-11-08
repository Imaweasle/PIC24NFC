#pragma once
#include "stdint.h"

uint64_t smallFontTable[128] = {
    0, // [0] Null Character
    0, // [1] Start of Heading (unused)
    0, // [2] Start of Text (unused)
    0, // [3] End of Text (unused)
    0, // [4] End of Transmission (unused)
    0, // [5] Enquiry (unused)
    0, // [6] ACK (unused)
    0, // [7] Bell, Alert (unused)
    0, // [8] Backspace (unused)
    0, // [9] Horizontal Tab (unused)
    0, // [10] Line Feed (unused)
    0, // [11] Vertical Tabulation (unused)
    0, // [12] Form Feed  (unused)
    0, // [13] Carriage Return (unused in font itself)
    0, // [14] Shift Out (unused)
    0, // [15] Shift In  (unused)
    0, // [16] Data Link Escape (unused)
    0, //
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

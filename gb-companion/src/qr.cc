/**
 * The MIT License (MIT)
 *
 * This library is written and maintained by Richard Moore.
 * Major parts were derived from Project Nayuki's library.
 *
 * Copyright (c) 2017 Richard Moore     (https://github.com/ricmoo/QRCode)
 * Copyright (c) 2017 Project Nayuki    (https://www.nayuki.io/page/qr-code-generator-library)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *  Special thanks to Nayuki (https://www.nayuki.io/) from which this library was
 *  heavily inspired and compared against.
 *
 *  See: https://github.com/nayuki/QR-Code-generator/tree/master/cpp
 */

#include "qr.h"

// #include <stdlib.h>
// #include <string.h>

// #pragma mark - Error Correction Lookup tables

#if LOCK_VERSION == 0

static const uint16_t NUM_ERROR_CORRECTION_CODEWORDS[4][40] = {
    // 1,  2,  3,  4,  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40    Error correction level
    { 10, 16, 26, 36, 48,  64,  72,  88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560,  588,  644,  700,  728,  784,  812,  868,  924,  980, 1036, 1064, 1120, 1204, 1260, 1316, 1372},  // Medium
    {  7, 10, 15, 20, 26,  36,  40,  48,  60,  72,  80,  96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300,  312,  336,  360,  390,  420,  450,  480,  510,  540,  570,  570,  600,  630,  660,  720,  750},  // Low
    { 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430},  // High
    { 13, 22, 36, 52, 72,  96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810,  870,  952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040},  // Quartile
};

static const uint8_t NUM_ERROR_CORRECTION_BLOCKS[4][40] = {
    // Version: (note that index 0 is for padding, and is set to an illegal value)
    // 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
    {  1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
    {  1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
    {  1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
    {  1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
};

static const uint16_t NUM_RAW_DATA_MODULES[40] = {
    //  1,   2,   3,   4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
      208, 359, 567, 807, 1079, 1383, 1568, 1936, 2336, 2768, 3232, 3728, 4256, 4651, 5243, 5867, 6523,
    //   18,   19,   20,   21,    22,    23,    24,    25,   26,    27,     28,    29,    30,    31,
       7211, 7931, 8683, 9252, 10068, 10916, 11796, 12708, 13652, 14628, 15371, 16411, 17483, 18587,
    //    32,    33,    34,    35,    36,    37,    38,    39,    40
       19723, 20891, 22091, 23008, 24272, 25568, 26896, 28256, 29648
};

// @TODO: Put other LOCK_VERSIONS here
#elif LOCK_VERSION == 3

static const int16_t NUM_ERROR_CORRECTION_CODEWORDS[4] = {
    26, 15, 44, 36
};

static const int8_t NUM_ERROR_CORRECTION_BLOCKS[4] = {
    1, 1, 2, 2
};

static const uint16_t NUM_RAW_DATA_MODULES = 567;

#elif LOCK_VERSION == 13

static const int16_t NUM_ERROR_CORRECTION_CODEWORDS[4] = {
    198, 104, 352, 288
};

static const int8_t NUM_ERROR_CORRECTION_BLOCKS[4] = {
    9, 4, 16, 12
};

static const uint16_t NUM_RAW_DATA_MODULES = 4256;

#else

#error Unsupported LOCK_VERSION (add it...)

#endif


static int max(int a, int b) {
    if (a > b) { return a; }
    return b;

    // // Branchless version: only works for unsigned
    // return ((a) ^ (((a) ^ (b)) & -((a) < (b))));
}


static int abs(int value) {
    if (value < 0) { return -value; }
    return value;

    // // Branchless version:
    // int mask = -(!(value & (0x1 << sizeof(int) * 8 - 1)));
    // return mask - (mask ^ value);
}

static void memset(uint8_t* dest, uint8_t value, size_t n) {
    size_t i;
    for (i = 0; i < n; ++i){
        dest[i] = value;
    }
}

static void memcpy(uint8_t *dest, const uint8_t *src, size_t n) {
    size_t i;
    for (i = 0; i < n; ++i){
        dest[i] = src[i];
    }
}

static size_t strlen(const char *str) {
    const char* ptr = str;
    for(; *ptr != 0; ptr++);
    return ptr - str;
}

// #pragma mark - Mode testing and conversion
#if !NO_ALPHANUMERIC
static int8_t getAlphanumeric(char c) {
    
    if (c >= '0' && c <= '9') { return (c - '0'); }
    if (c >= 'A' && c <= 'Z') { return (c - 'A' + 10); }
    
    switch (c) {
        case ' ': return 36;
        case '$': return 37;
        case '%': return 38;
        case '*': return 39;
        case '+': return 40;
        case '-': return 41;
        case '.': return 42;
        case '/': return 43;
        case ':': return 44;
    }
    
    return -1;
}

static bool isAlphanumeric(const char *text, uint16_t length) {
    while (length != 0) {
        if (getAlphanumeric(text[--length]) == -1) { return false; }
    }
    return true;
}
#endif

#if !NO_NUMERIC
static bool isNumeric(const char *text, uint16_t length) {
    while (length != 0) {
        char c = text[--length];
        if (c < '0' || c > '9') { return false; }
    }
    return true;
}
#endif


// #pragma mark - Counting

#if LOCK_VERSION == 0 || LOCK_VERSION > 26
const uint8_t MODE_BITS_LTE_V1_TO_V9_TABLE[3] = {  10, 9,  8  };
#endif
#if LOCK_VERSION == 0 || LOCK_VERSION > 9
const uint8_t MODE_BITS_LTE_V10_TO_V26_TABLE[3] = {  12, 11, 16 };
#endif
#if LOCK_VERSION <= 9
const uint8_t MODE_BITS_LTE_V27_TO_V40_TABLE[3] = {  14, 13, 16 };
#endif


// We store the following tightly packed (less 8) in modeInfo
//               <=9  <=26  <= 40
// NUMERIC      ( 10,   12,    14);
// ALPHANUMERIC (  9,   11,    13);
// BYTE         (  8,   16,    16);
static char getModeBits(uint8_t version, uint8_t mode) {
#if LOCK_VERSION == 0
    if (version > 26) {
        return MODE_BITS_LTE_V27_TO_V40_TABLE[mode];
    }
    if (version > 9) {
        return MODE_BITS_LTE_V10_TO_V26_TABLE[mode];
    }
    return MODE_BITS_LTE_V1_TO_V9_TABLE[mode];
#endif

#if LOCK_VERSION > 26
    return MODE_BITS_LTE_V27_TO_V40_TABLE[mode];
#elif LOCK_VERSION > 9
    return MODE_BITS_LTE_V10_TO_V26_TABLE[mode];
#else 
    return MODE_BITS_LTE_V1_TO_V9_TABLE[mode];
#endif
}


// #pragma mark - bit_bucket_t

typedef struct bit_bucket_t {
    union {
        uint32_t bitOffset;
        uint32_t bitWidth;
    };
    uint16_t capacityBytes;
    uint8_t *data;
} bit_bucket_t;

// These function are only necessary if no version has been locked in
#if LOCK_VERSION == 0
static uint16_t bb_getGridSizeBytes(uint8_t size) {
    return (((size * size) + 7) / 8);
}

static uint16_t bb_getBufferSizeBytes(uint32_t bits) {
    return ((bits + 7) / 8);
}
#endif

static void bb_initBuffer(bit_bucket_t *bitBuffer, uint8_t *data, uint32_t capacityBytes) {
    bitBuffer->bitOffset = 0;
    bitBuffer->capacityBytes = capacityBytes;
    bitBuffer->data = data;
    memset(data, 0, bitBuffer->capacityBytes);
}

static void bb_initGrid(bit_bucket_t *bitGrid, uint8_t *data, uint8_t size) {
    bitGrid->bitWidth = size;
#if LOCK_VERSION == 0
    bitGrid->capacityBytes = bb_getGridSizeBytes(size);
#else
    bitGrid->capacityBytes = bb_getGridSizeBytes(size);
#endif
    bitGrid->data = data;
    memset(data, 0, bitGrid->capacityBytes);
}

static void bb_appendBits(bit_bucket_t *bitBuffer, uint32_t val, uint8_t length) {
    uint32_t offset = bitBuffer->bitOffset;
    int8_t i;
    for (i = length - 1; i >= 0; i--, offset++) {
        bitBuffer->data[offset >> 3] |= ((val >> i) & 1) << (7 - (offset & 7));
    }
    bitBuffer->bitOffset = offset;
}

static void bb_setBit(bit_bucket_t *bitGrid, uint8_t x, uint8_t y, bool on) {
    uint32_t offset = y * bitGrid->bitWidth + x;
    uint8_t mask = 1 << (7 - (offset & 0x07));
    if (on) {
        bitGrid->data[offset >> 3] |= mask;
    } else {
        bitGrid->data[offset >> 3] &= ~mask;
    }
}

static void bb_invertBit(bit_bucket_t *bitGrid, uint8_t x, uint8_t y, bool invert) {
    uint32_t offset = y * bitGrid->bitWidth + x;
    uint8_t mask = 1 << (7 - (offset & 0x07));
    bool on = ((bitGrid->data[offset >> 3] & (1 << (7 - (offset & 0x07)))) != 0);
    if (on ^ invert) {
        bitGrid->data[offset >> 3] |= mask;
    } else {
        bitGrid->data[offset >> 3] &= ~mask;
    }
}

static bool bb_getBit(bit_bucket_t *bitGrid, uint8_t x, uint8_t y) {
    uint32_t offset = y * bitGrid->bitWidth + x;
    return (bitGrid->data[offset >> 3] & (1 << (7 - (offset & 0x07)))) != 0;
}


// #pragma mark - Drawing Patterns

// XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
// properties, calling applyMask(m) twice with the same value is equivalent to no change at all.
// This means it is possible to apply a mask, undo it, and try another mask. Note that a final
// well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
static void applyMask(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t mask) {
    uint8_t size = modules->bitWidth;
    uint8_t y;
    uint8_t x;
    bool invert;
    
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            if (bb_getBit(isFunction, x, y)) { continue; }
            
            invert = 0;
            switch (mask) {
                // case 0:  invert = (x + y) % 2 == 0;                    break;
                case 0:  invert = ((x + y) & 1) == 0;                  break;
                case 1:  invert = y % 2 == 0;                          break;
                case 2:  invert = x % 3 == 0;                          break;
                case 3:  invert = (x + y) % 3 == 0;                    break;
                case 4:  invert = (x / 3 + y / 2) % 2 == 0;            break;
                case 5:  invert = x * y % 2 + x * y % 3 == 0;          break;
                case 6:  invert = (x * y % 2 + x * y % 3) % 2 == 0;    break;
                case 7:  invert = ((x + y) % 2 + x * y % 3) % 2 == 0;  break;
            }
            bb_invertBit(modules, x, y, invert);
        }
    }
}

static void setFunctionModule(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t x, uint8_t y, bool on) {
    bb_setBit(modules, x, y, on);
    bb_setBit(isFunction, x, y, true);
}

// Draws a 9*9 finder pattern including the border separator, with the center module at (x, y).
static void drawFinderPattern(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t x, uint8_t y) {
    uint8_t size = modules->bitWidth;
    int8_t i, j;
    uint8_t dist;
    int16_t xx, yy;

    for (i = -4; i <= 4; i++) {
        for (j = -4; j <= 4; j++) {
            dist = max(abs(i), abs(j));  // Chebyshev/infinity norm
            xx = x + j;
            yy = y + i;
            if (0 <= xx && xx < size && 0 <= yy && yy < size) {
                setFunctionModule(modules, isFunction, xx, yy, dist != 2 && dist != 4);
            }
        }
    }
}

// Draws a 5*5 alignment pattern, with the center module at (x, y).
static void drawAlignmentPattern(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t x, uint8_t y) {
    int8_t i, j;
    for (i = -2; i <= 2; i++) {
        for (j = -2; j <= 2; j++) {
            setFunctionModule(modules, isFunction, x + j, y + i, max(abs(i), abs(j)) != 1);
        }
    }
}

// Draws two copies of the format bits (with its own error correction code)
// based on the given mask and this object's error correction level field.
static void drawFormatBits(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t ecc, uint8_t mask) {
    
    uint8_t size = modules->bitWidth;

    // Calculate error correction code and pack bits
    uint32_t data = ecc << 3 | mask;  // errCorrLvl is uint2, mask is uint3
    uint32_t rem = data;
    {
        int i;
        for (i = 0; i < 10; i++) {
            rem = (rem << 1) ^ ((rem >> 9) * 0x537);
        }
    }
    
    data = data << 10 | rem;
    data ^= 0x5412;  // uint15
    
    {
        // Draw first copy
        uint8_t i;
        for (i = 0; i <= 5; i++) {
            setFunctionModule(modules, isFunction, 8, i, ((data >> i) & 1) != 0);
        }
    }
    
    setFunctionModule(modules, isFunction, 8, 7, ((data >> 6) & 1) != 0);
    setFunctionModule(modules, isFunction, 8, 8, ((data >> 7) & 1) != 0);
    setFunctionModule(modules, isFunction, 7, 8, ((data >> 8) & 1) != 0);
    {
        uint8_t i;
        for (i = 9; i < 15; i++) {
            setFunctionModule(modules, isFunction, 14 - i, 8, ((data >> i) & 1) != 0);
        }
    }
    

    {
        // Draw second copy
        uint8_t i;
        for (i = 0; i <= 7; i++) {
            setFunctionModule(modules, isFunction, size - 1 - i, 8, ((data >> i) & 1) != 0);
        }
    }
    
    {
        uint8_t i;
        for (i = 8; i < 15; i++) {
            setFunctionModule(modules, isFunction, 8, size - 15 + i, ((data >> i) & 1) != 0);
        }
    }
    
    setFunctionModule(modules, isFunction, 8, size - 8, true);
}


// Draws two copies of the version bits (with its own error correction code),
// based on this object's version field (which only has an effect for 7 <= version <= 40).
static void drawVersion(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t version) {
    
    int8_t size = modules->bitWidth
    uint32_t rem = version;  // version is uint6, in the range [7, 40]

#if LOCK_VERSION != 0 && LOCK_VERSION < 7
    return;
    
#else
    if (version < 7) { return; }
    
    {
        // Calculate error correction code and pack bits
        uint8_t i;
        for (i = 0; i < 12; i++) {
            rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);
        }
    }
    
    {
        uint32_t data = ((uint32_t)version) << 12 | rem;  // uint18
        uint8_t i, a, b;
        bool bit;
        // Draw two copies
        for (i = 0; i < 18; i++) {
            bit = ((data >> i) & 1) != 0;
            a = size - 11 + i % 3;
            b = i / 3;
            setFunctionModule(modules, isFunction, a, b, bit);
            setFunctionModule(modules, isFunction, b, a, bit);
        }
    }
    
#endif
}

static void drawFunctionPatterns(bit_bucket_t *modules, bit_bucket_t *isFunction, uint8_t version, uint8_t ecc) {
    
    uint8_t size = modules->bitWidth;

    {
        uint8_t i;
        // Draw the horizontal and vertical timing patterns
        for (i = 0; i < size; i++) {
            setFunctionModule(modules, isFunction, 6, i, i % 2 == 0);
            setFunctionModule(modules, isFunction, i, 6, i % 2 == 0);
        }

    }
    
    // Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
    drawFinderPattern(modules, isFunction, 3, 3);
    drawFinderPattern(modules, isFunction, size - 4, 3);
    drawFinderPattern(modules, isFunction, 3, size - 4);
    
#if LOCK_VERSION == 0 || LOCK_VERSION > 1

#if LOCK_VERSION == 0
#define _version  version
#else
#define _version  LOCK_VERSION
#endif
#define _align_count (_version / 7 + 2)

    if (_version > 1) {

        // Draw the numerous alignment patterns
        uint8_t step;
        if (_version != 32) {
            step = (_version * 4 + _align_count * 2 + 1) / (2 * _align_count - 2) * 2;  // ceil((size - 13) / (2*numAlign - 2)) * 2
        } else { // C-C-C-Combo breaker!
            step = 26;
        }
        
        {
            uint8_t alignPositionIndex = _align_count - 1;
#if LOCK_VERSION == 0
            uint8_t alignPosition[40];
#else
            uint8_t alignPosition[_align_count];
#endif
            
            alignPosition[0] = 6;
            {
                uint8_t size = _version * 4 + 17;
                uint8_t pos, i, j;
                for (i = 0, pos = size - 7; i < _align_count - 1; i++, pos -= step) {
                    alignPosition[alignPositionIndex--] = pos;
                }
                
                for (i = 0; i < _align_count; i++) {
                    for (j = 0; j < _align_count; j++) {
                        if ((i == 0 && j == 0) || (i == 0 && j == _align_count - 1) || (i == _align_count - 1 && j == 0)) {
                            continue;  // Skip the three finder corners
                        } else {
                            drawAlignmentPattern(modules, isFunction, alignPosition[i], alignPosition[j]);
                        }
                    }
                }
            }
        }
    }
    
#endif
    
    // Draw configuration data
    drawFormatBits(modules, isFunction, ecc, 0);  // Dummy mask value; overwritten later in the constructor
    drawVersion(modules, isFunction, version);
}


// Draws the given sequence of 8-bit codewords (data and error correction) onto the entire
// data area of this QR Code symbol. Function modules need to be marked off before this is called.
static void drawCodewords(bit_bucket_t *modules, bit_bucket_t *isFunction, bit_bucket_t *codewords) {
    
    uint8_t bitLength = codewords->bitWidth;
    uint8_t *data = codewords->data;
    
    uint8_t size = modules->bitWidth;
    
    // Bit index into the data
    uint16_t i = 0;

    int16_t right;
    uint8_t vert;
    int j;
    uint8_t x, y;
    bool upwards;
    
    // Do the funny zigzag scan
    for (right = size - 1; right >= 1; right -= 2) {  // Index of right column in each column pair
        if (right == 6) { right = 5; }
        
        for (vert = 0; vert < size; vert++) {  // Vertical counter
            for (j = 0; j < 2; j++) {
                x = right - j;  // Actual x coordinate
                upwards = ((right & 2) == 0) ^ (x < 6);
                y = upwards ? size - 1 - vert : vert;  // Actual y coordinate
                if (!bb_getBit(isFunction, x, y) && i < bitLength) {
                    bb_setBit(modules, x, y, ((data[i >> 3] >> (7 - (i & 7))) & 1) != 0);
                    i++;
                }
                // If there are any remainder bits (0 to 7), they are already
                // set to 0/false/white when the grid of modules was initialized
            }
        }
    }
}



// // #pragma mark - Penalty Calculation

// #define PENALTY_N1      3
// #define PENALTY_N2      3
// #define PENALTY_N3     40
// #define PENALTY_N4     10

// // Calculates and returns the penalty score based on state of this QR Code's current modules.
// // This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
// // @TODO: This can be optimized by working with the bytes instead of bits.
// static uint32_t getPenaltyScore(bit_bucket_t *modules) {
//     uint32_t result = 0;
    
//     uint8_t size = modules->bitOffsetOrWidth;
    
//     {
//         uint8_t y, x, runX;
//         bool colorX, cx;
//         // Adjacent modules in row having same color
//         for (y = 0; y < size; y++) {   
//             colorX = bb_getBit(modules, 0, y);
//             for (x = 1, runX = 1; x < size; x++) {
//                 cx = bb_getBit(modules, x, y);
//                 if (cx != colorX) {
//                     colorX = cx;
//                     runX = 1;
                    
//                 } else {
//                     runX++;
//                     if (runX == 5) {
//                         result += PENALTY_N1;
//                     } else if (runX > 5) {
//                         result++;
//                     }
//                 }
//             }
//         }
//     }
    
//     {
//         uint8_t y, x, runY;
//         bool colorY, cy;
//         // Adjacent modules in column having same color
//         for (x = 0; x < size; x++) {
//             colorY = bb_getBit(modules, x, 0);
//             for (y = 1, runY = 1; y < size; y++) {
//                 cy = bb_getBit(modules, x, y);
//                 if (cy != colorY) {
//                     colorY = cy;
//                     runY = 1;
//                 } else {
//                     runY++;
//                     if (runY == 5) {
//                         result += PENALTY_N1;
//                     } else if (runY > 5) {
//                         result++;
//                     }
//                 }
//             }
//         }
//     }

//     {
//         uint16_t black = 0, bitsRow, bitsCol;
//         uint8_t y, x;
//         bool color, colorUL, colorUR, colorL;

//         for (y = 0; y < size; y++) {
//             bitsRow = 0, bitsCol = 0;
//             for (x = 0; x < size; x++) {
//                 color = bb_getBit(modules, x, y);

//                 // 2*2 blocks of modules having same color
//                 if (x > 0 && y > 0) {
//                     colorUL = bb_getBit(modules, x - 1, y - 1);
//                     colorUR = bb_getBit(modules, x, y - 1);
//                     colorL = bb_getBit(modules, x - 1, y);
//                     if (color == colorUL && color == colorUR && color == colorL) {
//                         result += PENALTY_N2;
//                     }
//                 }

//                 // Finder-like pattern in rows and columns
//                 bitsRow = ((bitsRow << 1) & 0x7FF) | color;
//                 bitsCol = ((bitsCol << 1) & 0x7FF) | bb_getBit(modules, y, x);

//                 // Needs 11 bits accumulated
//                 if (x >= 10) {
//                     if (bitsRow == 0x05D || bitsRow == 0x5D0) {
//                         result += PENALTY_N3;
//                     }
//                     if (bitsCol == 0x05D || bitsCol == 0x5D0) {
//                         result += PENALTY_N3;
//                     }
//                 }

//                 // Balance of black and white modules
//                 if (color) { black++; }
//             }
//         }
//         {
//             // Find smallest k such that (45-5k)% <= dark/total <= (55+5k)%
//             uint16_t total = size * size, k;
//             for (k = 0; black * 20 < (9 - k) * total || black * 20 > (11 + k) * total; k++) {
//                 result += PENALTY_N4;
//             }
//         }
//     }

    
//     return result;
// }


// #pragma mark - Reed-Solomon Generator

static uint8_t rs_multiply(uint8_t x, uint8_t y) {
    // Russian peasant multiplication
    // See: https://en.wikipedia.org/wiki/Ancient_Egyptian_multiplication
    uint16_t z = 0;
    int8_t i;
    for (i = 7; i >= 0; i--) {
        z = (z << 1) ^ ((z >> 7) * 0x11D);
        z ^= ((y >> i) & 1) * x;
    }
    return z;
}

static void rs_init(uint8_t degree, uint8_t *coeff) {
    // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{degree-1}),
    // drop the highest term, and store the rest of the coefficients in order of descending powers.
    // Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
    uint16_t root = 1;
    uint8_t i, j;


    memset(coeff, 0, degree);
    coeff[degree - 1] = 1;
    
    for (i = 0; i < degree; i++) {
        // Multiply the current product by (x - r^i)
        for (j = 0; j < degree; j++) {
            coeff[j] = rs_multiply(coeff[j], root);
            if (j + 1 < degree) {
                coeff[j] ^= coeff[j + 1];
            }
        }
        root = (root << 1) ^ ((root >> 7) * 0x11D);  // Multiply by 0x02 mod GF(2^8/0x11D)
    }
}

static void rs_getRemainder(uint8_t degree, uint8_t *coeff, uint8_t *data, uint8_t length, uint8_t *result, uint8_t stride) {
    // Compute the remainder by performing polynomial division
    
    //for (uint8_t i = 0; i < degree; i++) { result[] = 0; }
    //memset(result, 0, degree);
    uint8_t i, factor, j;

    for (i = 0; i < length; i++) {
        factor = data[i] ^ result[0];
        for (j = 1; j < degree; j++) {
            result[(j - 1) * stride] = result[j * stride];
        }
        result[(degree - 1) * stride] = 0;
        
        for (j = 0; j < degree; j++) {
            result[j * stride] ^= rs_multiply(coeff[j], factor);
        }
    }
}



// #pragma mark - QrCode

static int8_t encodeDataCodewords(bit_bucket_t *dataCodewords, const uint8_t *text, uint16_t length, uint8_t version) {
    int8_t mode = MODE_BYTE;
    
#if !NO_NUMERIC
    if (isNumeric((char*)text, length)) {
        uint16_t accumData = 0;
        uint8_t accumCount = 0;
        uint16_t i;

        mode = MODE_NUMERIC;
        bb_appendBits(dataCodewords, 1 << MODE_NUMERIC, 4);
        bb_appendBits(dataCodewords, length, getModeBits(version, MODE_NUMERIC));

        for (i = 0; i < length; i++) {
            accumData = accumData * 10 + ((char)(text[i]) - '0');
            accumCount++;
            if (accumCount == 3) {
                bb_appendBits(dataCodewords, accumData, 10);
                accumData = 0;
                accumCount = 0;
            }
        }
        
        // 1 or 2 digits remaining
        if (accumCount > 0) {
            bb_appendBits(dataCodewords, accumData, accumCount * 3 + 1);
        }
        
    } 
    else
#endif
#if !NO_ALPHANUMERIC
    if (isAlphanumeric((char*)text, length)) {
        uint16_t accumData = 0;
        uint8_t accumCount = 0;
        uint16_t i;

        mode = MODE_ALPHANUMERIC;
        bb_appendBits(dataCodewords, 1 << MODE_ALPHANUMERIC, 4);
        bb_appendBits(dataCodewords, length, getModeBits(version, MODE_ALPHANUMERIC));

        for (i = 0; i  < length; i++) {
            accumData = accumData * 45 + getAlphanumeric((char)(text[i]));
            accumCount++;
            if (accumCount == 2) {
                bb_appendBits(dataCodewords, accumData, 11);
                accumData = 0;
                accumCount = 0;
            }
        }
        
        // 1 character remaining
        if (accumCount > 0) {
            bb_appendBits(dataCodewords, accumData, 6);
        }
        
    } 
    else
#endif
    {
        bb_appendBits(dataCodewords, 1 << MODE_BYTE, 4);
        bb_appendBits(dataCodewords, length, getModeBits(version, MODE_BYTE));

        {
            uint16_t i;
            for (i = 0; i < length; i++) {
                bb_appendBits(dataCodewords, (char)(text[i]), 8);
            }
        }
    }
    
    //bb_setBits(dataCodewords, length, 4, getModeBits(version, mode));
    
    return mode;
}

static void performErrorCorrection(uint8_t version, uint8_t ecc, bit_bucket_t *data) {
    
    // See: http://www.thonky.com/qr-code-tutorial/structure-final-message
    
#if LOCK_VERSION == 0
    uint8_t numBlocks = NUM_ERROR_CORRECTION_BLOCKS[ecc][version - 1];
    uint16_t totalEcc = NUM_ERROR_CORRECTION_CODEWORDS[ecc][version - 1];
    uint16_t moduleCount = NUM_RAW_DATA_MODULES[version - 1];
#else
    uint8_t numBlocks = NUM_ERROR_CORRECTION_BLOCKS[ecc];
    uint16_t totalEcc = NUM_ERROR_CORRECTION_CODEWORDS[ecc];
    uint16_t moduleCount = NUM_RAW_DATA_MODULES;
#endif
    
    uint8_t blockEccLen = totalEcc / numBlocks;
    uint8_t numShortBlocks = numBlocks - moduleCount / 8 % numBlocks;
    uint8_t shortBlockLen = moduleCount / 8 / numBlocks;
    
    uint8_t shortDataBlockLen = shortBlockLen - blockEccLen;
    
#if LOCK_VERSION == 0
    uint8_t result[QRCODE_BUFFER_SIZE(40)];    
    uint8_t coeff[30];
#else
    uint8_t result[QRCODE_BUFFER_SIZE(LOCK_VERSION)];
    uint8_t coeff[30];
#endif

    uint16_t offset = 0;
    uint8_t *dataBytes = data->data;

    memset(result, 0, sizeof(result));
    rs_init(blockEccLen, coeff);
    
    {
        uint8_t i, stride, blockNum;
        uint16_t index;
        // Interleave all short blocks
        for (i = 0; i < shortDataBlockLen; i++) {
            index = i;
            stride = shortDataBlockLen;
            for (blockNum = 0; blockNum < numBlocks; blockNum++) {
                result[offset++] = dataBytes[index];
                
    #if LOCK_VERSION == 0 || LOCK_VERSION >= 5
                if (blockNum == numShortBlocks) { stride++; }
    #endif
                index += stride;
            }
        }
    }
    
    // Version less than 5 only have short blocks
#if LOCK_VERSION == 0 || LOCK_VERSION >= 5
    {
        // Interleave long blocks
        uint16_t index = shortDataBlockLen * (numShortBlocks + 1);
        uint8_t stride = shortDataBlockLen, blockNum;
        for (blockNum = 0; blockNum < numBlocks - numShortBlocks; blockNum++) {
            result[offset++] = dataBytes[index];
            
            if (blockNum == 0) { stride++; }
            index += stride;
        }
    }
#endif
    
    {
        // Add all ecc blocks, interleaved
        uint8_t blockSize = shortDataBlockLen, blockNum;
        for (blockNum = 0; blockNum < numBlocks; blockNum++) {
            
    #if LOCK_VERSION == 0 || LOCK_VERSION >= 5
            if (blockNum == numShortBlocks) { blockSize++; }
    #endif
            rs_getRemainder(blockEccLen, coeff, dataBytes, blockSize, &result[offset + blockNum], numBlocks);
            dataBytes += blockSize;
        }
        
        memcpy(data->data, result, data->capacityBytes);
        data->bitOffsetOrWidth = moduleCount;
    }
}

// We store the Format bits tightly packed into a single byte (each of the 4 modes is 2 bits)
// The format bits can be determined by ECC_FORMAT_BITS >> (2 * ecc)
// static const uint8_t ECC_FORMAT_BITS = (0x02 << 6) | (0x03 << 4) | (0x00 << 2) | (0x01 << 0);
#define ECC_FORMAT_BITS ((uint8_t)((0x02 << 6) | (0x03 << 4) | (0x00 << 2) | (0x01 << 0)))


// #pragma mark - Public QRCode functions

uint16_t qrcode_getBufferSize(uint8_t version) {
    return bb_getGridSizeBytes(4 * version + 17);
}

// @TODO: Return error if data is too big.
int8_t qrcode_initBytes(QRCode *qrcode, uint8_t *modules, uint8_t version, uint8_t ecc, uint8_t *data, uint16_t length) {
    uint8_t size = version * 4 + 17;
    uint8_t mode, padByte;
    uint32_t padding;

    bit_bucket_t codewords, modulesGrid, isFunctionGrid;
#if LOCK_VERSION == 0
    uint8_t codewordBytes[QRCODE_BUFFER_SIZE(40)];
    uint8_t isFunctionGridBytes[QRCODE_BUFFER_SIZE(40)];
#else
    uint8_t codewordBytes[QRCODE_BUFFER_SIZE(LOCK_VERSION)];
    uint8_t isFunctionGridBytes[QRCODE_BUFFER_SIZE(LOCK_VERSION)];
#endif

    uint8_t eccFormatBits = (ECC_FORMAT_BITS >> (2 * ecc)) & 0x03;
    
#if LOCK_VERSION == 0
    uint16_t moduleCount = NUM_RAW_DATA_MODULES[version - 1];
    uint16_t dataCapacity = moduleCount / 8 - NUM_ERROR_CORRECTION_CODEWORDS[eccFormatBits][version - 1];
#else
    version = LOCK_VERSION;
    uint16_t moduleCount = NUM_RAW_DATA_MODULES;
    uint16_t dataCapacity = moduleCount / 8 - NUM_ERROR_CORRECTION_CODEWORDS[eccFormatBits];
#endif
    
    qrcode->version = version;
    qrcode->size = size;
    qrcode->ecc = ecc;
    qrcode->modules = modules;
    
    // struct bit_bucket_t codewords;
    // uint8_t codewordBytes[bb_getBufferSizeBytes(moduleCount)];
    bb_initBuffer(&codewords, codewordBytes, (int32_t)sizeof(codewordBytes));
    
    // Place the data code words into the buffer
    mode = encodeDataCodewords(&codewords, data, length, version);
    
    if (mode < 0) { return -1; }
    qrcode->mode = mode;
    
    // Add terminator and pad up to a byte if applicable
    padding = (dataCapacity * 8) - codewords.bitOffsetOrWidth;
    if (padding > 4) { padding = 4; }
    bb_appendBits(&codewords, 0, padding);
    bb_appendBits(&codewords, 0, (8 - codewords.bitOffsetOrWidth % 8) % 8);

    // Pad with alternate bytes until data capacity is reached
    for (padByte = 0xEC; codewords.bitOffsetOrWidth < (dataCapacity * 8); padByte ^= 0xEC ^ 0x11) {
        bb_appendBits(&codewords, padByte, 8);
    }

    // bit_bucket_t modulesGrid;
    bb_initGrid(&modulesGrid, modules, size);
    
    // bit_bucket_t isFunctionGrid;
    // uint8_t isFunctionGridBytes[bb_getGridSizeBytes(size)];
    bb_initGrid(&isFunctionGrid, isFunctionGridBytes, size);

    // Pretty fast to get to here (~1 sec)
    
    // Draw function patterns, draw all codewords, do masking
    drawFunctionPatterns(&modulesGrid, &isFunctionGrid, version, eccFormatBits);
    performErrorCorrection(version, eccFormatBits, &codewords);
    drawCodewords(&modulesGrid, &isFunctionGrid, &codewords);

    // To get to here, takes roughly ~5-10s

    // return 0; /// DELETE ME
    
    {
        // // Find the best (lowest penalty) mask
        uint8_t mask = 0;

        // // This is super slow. Just take mask 0 for now

        // int32_t minPenalty = INT32_MAX;
        // uint8_t i;
        // for (i = 0; i < 8; i++) {
        //     drawFormatBits(&modulesGrid, &isFunctionGrid, eccFormatBits, i);
        //     applyMask(&modulesGrid, &isFunctionGrid, i);
        //     {
        //         int penalty = getPenaltyScore(&modulesGrid);
        //         if (penalty < minPenalty) {
        //             mask = i;
        //             minPenalty = penalty;
        //         }
        //     }
        //     applyMask(&modulesGrid, &isFunctionGrid, i);  // Undoes the mask due to XOR
        // }
        
        qrcode->mask = mask;
        
        // Overwrite old format bits
        drawFormatBits(&modulesGrid, &isFunctionGrid, eccFormatBits, mask);
        
        // Apply the final choice of mask
        applyMask(&modulesGrid, &isFunctionGrid, mask);
    }

    return 0;
}

int8_t qrcode_initText(QRCode *qrcode, uint8_t *modules, uint8_t version, uint8_t ecc, const char *data) {
    return qrcode_initBytes(qrcode, modules, version, ecc, (uint8_t*)data, strlen(data));
}

bool qrcode_getModule(QRCode *qrcode, uint8_t x, uint8_t y) {
    if (x < 0 || x >= qrcode->size || y < 0 || y >= qrcode->size) {
        return false;
    }
    {
        uint32_t offset = y * qrcode->size + x;
        return (qrcode->modules[offset >> 3] & (1 << (7 - (offset & 0x07)))) != 0;
    }
}

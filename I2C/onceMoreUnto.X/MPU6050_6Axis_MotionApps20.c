#define MPU6050_INCLUDE_DMP_MOTIONAPPS20

#include "main.h"

uint16_t dmpPacketSize;
uint8_t *dmpPacketBuffer;

#define MPU6050_DMP_CODE_SIZE 1929  // dmpMemory[]
#define MPU6050_DMP_CONFIG_SIZE 192 // dmpConfig[]
#define MPU6050_DMP_UPDATES_SIZE 47 // dmpUpdates[]
#define FRAMESIZE 20
#define NONE 0
#define IMU_WARM 1
#define CRASH 2
#define CANCEL 3

#define UP 5
#define DN 6
#define LEFT 7
#define RIGHT 8
#define GO 9

/* ================================================================================================ *
 | Default MotionApps v2.0 42-byte FIFO packet structure:                                           |
 |                                                                                                  |
 | [QUAT W][      ][QUAT X][      ][QUAT Y][      ][QUAT Z][      ][GYRO X][      ][GYRO Y][      ] |
 |   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  |
 |                                                                                                  |
 | [GYRO Z][      ][ACC X ][      ][ACC Y ][      ][ACC Z ][      ][      ]                         |
 |  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41                          |
 * ================================================================================================ */

const unsigned char dmpMemory[MPU6050_DMP_CODE_SIZE] = {
    // bank 0, 256 bytes
    0xFB, 0x00, 0x00, 0x3E, 0x00, 0x0B, 0x00, 0x36, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0xFA, 0x80, 0x00, 0x0B, 0x12, 0x82, 0x00, 0x01,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x28, 0x00, 0x00, 0xFF, 0xFF, 0x45, 0x81, 0xFF, 0xFF, 0xFA, 0x72, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x7F, 0xFF, 0xFF, 0xFE, 0x80, 0x01,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3E, 0x03, 0x30, 0x40, 0x00, 0x00, 0x00, 0x02, 0xCA, 0xE3, 0x09, 0x3E, 0x80, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
    0x41, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x2A, 0x00, 0x00, 0x16, 0x55, 0x00, 0x00, 0x21, 0x82,
    0xFD, 0x87, 0x26, 0x50, 0xFD, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6F, 0x00, 0x02, 0x65, 0x32, 0x00, 0x00, 0x5E, 0xC0,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFB, 0x8C, 0x6F, 0x5D, 0xFD, 0x5D, 0x08, 0xD9, 0x00, 0x7C, 0x73, 0x3B, 0x00, 0x6C, 0x12, 0xCC,
    0x32, 0x00, 0x13, 0x9D, 0x32, 0x00, 0xD0, 0xD6, 0x32, 0x00, 0x08, 0x00, 0x40, 0x00, 0x01, 0xF4,
    0xFF, 0xE6, 0x80, 0x79, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xD6, 0x00, 0x00, 0x27, 0x10,

    // bank 1, 256 bytes
    0xFB, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0x36, 0xFF, 0xBC, 0x30, 0x8E, 0x00, 0x05, 0xFB, 0xF0, 0xFF, 0xD9, 0x5B, 0xC8,
    0xFF, 0xD0, 0x9A, 0xBE, 0x00, 0x00, 0x10, 0xA9, 0xFF, 0xF4, 0x1E, 0xB2, 0x00, 0xCE, 0xBB, 0xF7,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x0C,
    0xFF, 0xC2, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0xCF, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x68, 0xB6, 0x79, 0x35, 0x28, 0xBC, 0xC6, 0x7E, 0xD1, 0x6C,
    0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB2, 0x6A, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x25, 0x4D, 0x00, 0x2F, 0x70, 0x6D, 0x00, 0x00, 0x05, 0xAE, 0x00, 0x0C, 0x02, 0xD0,

    // bank 2, 256 bytes
    0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0xFF, 0xEF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // bank 3, 256 bytes
    0xD8, 0xDC, 0xBA, 0xA2, 0xF1, 0xDE, 0xB2, 0xB8, 0xB4, 0xA8, 0x81, 0x91, 0xF7, 0x4A, 0x90, 0x7F,
    0x91, 0x6A, 0xF3, 0xF9, 0xDB, 0xA8, 0xF9, 0xB0, 0xBA, 0xA0, 0x80, 0xF2, 0xCE, 0x81, 0xF3, 0xC2,
    0xF1, 0xC1, 0xF2, 0xC3, 0xF3, 0xCC, 0xA2, 0xB2, 0x80, 0xF1, 0xC6, 0xD8, 0x80, 0xBA, 0xA7, 0xDF,
    0xDF, 0xDF, 0xF2, 0xA7, 0xC3, 0xCB, 0xC5, 0xB6, 0xF0, 0x87, 0xA2, 0x94, 0x24, 0x48, 0x70, 0x3C,
    0x95, 0x40, 0x68, 0x34, 0x58, 0x9B, 0x78, 0xA2, 0xF1, 0x83, 0x92, 0x2D, 0x55, 0x7D, 0xD8, 0xB1,
    0xB4, 0xB8, 0xA1, 0xD0, 0x91, 0x80, 0xF2, 0x70, 0xF3, 0x70, 0xF2, 0x7C, 0x80, 0xA8, 0xF1, 0x01,
    0xB0, 0x98, 0x87, 0xD9, 0x43, 0xD8, 0x86, 0xC9, 0x88, 0xBA, 0xA1, 0xF2, 0x0E, 0xB8, 0x97, 0x80,
    0xF1, 0xA9, 0xDF, 0xDF, 0xDF, 0xAA, 0xDF, 0xDF, 0xDF, 0xF2, 0xAA, 0xC5, 0xCD, 0xC7, 0xA9, 0x0C,
    0xC9, 0x2C, 0x97, 0x97, 0x97, 0x97, 0xF1, 0xA9, 0x89, 0x26, 0x46, 0x66, 0xB0, 0xB4, 0xBA, 0x80,
    0xAC, 0xDE, 0xF2, 0xCA, 0xF1, 0xB2, 0x8C, 0x02, 0xA9, 0xB6, 0x98, 0x00, 0x89, 0x0E, 0x16, 0x1E,
    0xB8, 0xA9, 0xB4, 0x99, 0x2C, 0x54, 0x7C, 0xB0, 0x8A, 0xA8, 0x96, 0x36, 0x56, 0x76, 0xF1, 0xB9,
    0xAF, 0xB4, 0xB0, 0x83, 0xC0, 0xB8, 0xA8, 0x97, 0x11, 0xB1, 0x8F, 0x98, 0xB9, 0xAF, 0xF0, 0x24,
    0x08, 0x44, 0x10, 0x64, 0x18, 0xF1, 0xA3, 0x29, 0x55, 0x7D, 0xAF, 0x83, 0xB5, 0x93, 0xAF, 0xF0,
    0x00, 0x28, 0x50, 0xF1, 0xA3, 0x86, 0x9F, 0x61, 0xA6, 0xDA, 0xDE, 0xDF, 0xD9, 0xFA, 0xA3, 0x86,
    0x96, 0xDB, 0x31, 0xA6, 0xD9, 0xF8, 0xDF, 0xBA, 0xA6, 0x8F, 0xC2, 0xC5, 0xC7, 0xB2, 0x8C, 0xC1,
    0xB8, 0xA2, 0xDF, 0xDF, 0xDF, 0xA3, 0xDF, 0xDF, 0xDF, 0xD8, 0xD8, 0xF1, 0xB8, 0xA8, 0xB2, 0x86,

    // bank 4, 256 bytes
    0xB4, 0x98, 0x0D, 0x35, 0x5D, 0xB8, 0xAA, 0x98, 0xB0, 0x87, 0x2D, 0x35, 0x3D, 0xB2, 0xB6, 0xBA,
    0xAF, 0x8C, 0x96, 0x19, 0x8F, 0x9F, 0xA7, 0x0E, 0x16, 0x1E, 0xB4, 0x9A, 0xB8, 0xAA, 0x87, 0x2C,
    0x54, 0x7C, 0xB9, 0xA3, 0xDE, 0xDF, 0xDF, 0xA3, 0xB1, 0x80, 0xF2, 0xC4, 0xCD, 0xC9, 0xF1, 0xB8,
    0xA9, 0xB4, 0x99, 0x83, 0x0D, 0x35, 0x5D, 0x89, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0xB5, 0x93, 0xA3,
    0x0E, 0x16, 0x1E, 0xA9, 0x2C, 0x54, 0x7C, 0xB8, 0xB4, 0xB0, 0xF1, 0x97, 0x83, 0xA8, 0x11, 0x84,
    0xA5, 0x09, 0x98, 0xA3, 0x83, 0xF0, 0xDA, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xD8, 0xF1, 0xA5,
    0x29, 0x55, 0x7D, 0xA5, 0x85, 0x95, 0x02, 0x1A, 0x2E, 0x3A, 0x56, 0x5A, 0x40, 0x48, 0xF9, 0xF3,
    0xA3, 0xD9, 0xF8, 0xF0, 0x98, 0x83, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0x97, 0x82, 0xA8, 0xF1,
    0x11, 0xF0, 0x98, 0xA2, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xDA, 0xF3, 0xDE, 0xD8, 0x83, 0xA5,
    0x94, 0x01, 0xD9, 0xA3, 0x02, 0xF1, 0xA2, 0xC3, 0xC5, 0xC7, 0xD8, 0xF1, 0x84, 0x92, 0xA2, 0x4D,
    0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
    0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0x93, 0xA3, 0x4D,
    0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
    0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0xA8, 0x8A, 0x9A,
    0xF0, 0x28, 0x50, 0x78, 0x9E, 0xF3, 0x88, 0x18, 0xF1, 0x9F, 0x1D, 0x98, 0xA8, 0xD9, 0x08, 0xD8,
    0xC8, 0x9F, 0x12, 0x9E, 0xF3, 0x15, 0xA8, 0xDA, 0x12, 0x10, 0xD8, 0xF1, 0xAF, 0xC8, 0x97, 0x87,

    // bank 5, 256 bytes
    0x34, 0xB5, 0xB9, 0x94, 0xA4, 0x21, 0xF3, 0xD9, 0x22, 0xD8, 0xF2, 0x2D, 0xF3, 0xD9, 0x2A, 0xD8,
    0xF2, 0x35, 0xF3, 0xD9, 0x32, 0xD8, 0x81, 0xA4, 0x60, 0x60, 0x61, 0xD9, 0x61, 0xD8, 0x6C, 0x68,
    0x69, 0xD9, 0x69, 0xD8, 0x74, 0x70, 0x71, 0xD9, 0x71, 0xD8, 0xB1, 0xA3, 0x84, 0x19, 0x3D, 0x5D,
    0xA3, 0x83, 0x1A, 0x3E, 0x5E, 0x93, 0x10, 0x30, 0x81, 0x10, 0x11, 0xB8, 0xB0, 0xAF, 0x8F, 0x94,
    0xF2, 0xDA, 0x3E, 0xD8, 0xB4, 0x9A, 0xA8, 0x87, 0x29, 0xDA, 0xF8, 0xD8, 0x87, 0x9A, 0x35, 0xDA,
    0xF8, 0xD8, 0x87, 0x9A, 0x3D, 0xDA, 0xF8, 0xD8, 0xB1, 0xB9, 0xA4, 0x98, 0x85, 0x02, 0x2E, 0x56,
    0xA5, 0x81, 0x00, 0x0C, 0x14, 0xA3, 0x97, 0xB0, 0x8A, 0xF1, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9,
    0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x84, 0x0D, 0xDA, 0x0E, 0xD8, 0xA3, 0x29, 0x83, 0xDA,
    0x2C, 0x0E, 0xD8, 0xA3, 0x84, 0x49, 0x83, 0xDA, 0x2C, 0x4C, 0x0E, 0xD8, 0xB8, 0xB0, 0xA8, 0x8A,
    0x9A, 0xF5, 0x20, 0xAA, 0xDA, 0xDF, 0xD8, 0xA8, 0x40, 0xAA, 0xD0, 0xDA, 0xDE, 0xD8, 0xA8, 0x60,
    0xAA, 0xDA, 0xD0, 0xDF, 0xD8, 0xF1, 0x97, 0x86, 0xA8, 0x31, 0x9B, 0x06, 0x99, 0x07, 0xAB, 0x97,
    0x28, 0x88, 0x9B, 0xF0, 0x0C, 0x20, 0x14, 0x40, 0xB8, 0xB0, 0xB4, 0xA8, 0x8C, 0x9C, 0xF0, 0x04,
    0x28, 0x51, 0x79, 0x1D, 0x30, 0x14, 0x38, 0xB2, 0x82, 0xAB, 0xD0, 0x98, 0x2C, 0x50, 0x50, 0x78,
    0x78, 0x9B, 0xF1, 0x1A, 0xB0, 0xF0, 0x8A, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x8B, 0x29, 0x51, 0x79,
    0x8A, 0x24, 0x70, 0x59, 0x8B, 0x20, 0x58, 0x71, 0x8A, 0x44, 0x69, 0x38, 0x8B, 0x39, 0x40, 0x68,
    0x8A, 0x64, 0x48, 0x31, 0x8B, 0x30, 0x49, 0x60, 0xA5, 0x88, 0x20, 0x09, 0x71, 0x58, 0x44, 0x68,

    // bank 6, 256 bytes
    0x11, 0x39, 0x64, 0x49, 0x30, 0x19, 0xF1, 0xAC, 0x00, 0x2C, 0x54, 0x7C, 0xF0, 0x8C, 0xA8, 0x04,
    0x28, 0x50, 0x78, 0xF1, 0x88, 0x97, 0x26, 0xA8, 0x59, 0x98, 0xAC, 0x8C, 0x02, 0x26, 0x46, 0x66,
    0xF0, 0x89, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31,
    0xA9, 0x88, 0x09, 0x20, 0x59, 0x70, 0xAB, 0x11, 0x38, 0x40, 0x69, 0xA8, 0x19, 0x31, 0x48, 0x60,
    0x8C, 0xA8, 0x3C, 0x41, 0x5C, 0x20, 0x7C, 0x00, 0xF1, 0x87, 0x98, 0x19, 0x86, 0xA8, 0x6E, 0x76,
    0x7E, 0xA9, 0x99, 0x88, 0x2D, 0x55, 0x7D, 0x9E, 0xB9, 0xA3, 0x8A, 0x22, 0x8A, 0x6E, 0x8A, 0x56,
    0x8A, 0x5E, 0x9F, 0xB1, 0x83, 0x06, 0x26, 0x46, 0x66, 0x0E, 0x2E, 0x4E, 0x6E, 0x9D, 0xB8, 0xAD,
    0x00, 0x2C, 0x54, 0x7C, 0xF2, 0xB1, 0x8C, 0xB4, 0x99, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0x81, 0x91,
    0xAC, 0x38, 0xAD, 0x3A, 0xB5, 0x83, 0x91, 0xAC, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9, 0x48, 0xD8,
    0x6D, 0xD9, 0x68, 0xD8, 0x8C, 0x9D, 0xAE, 0x29, 0xD9, 0x04, 0xAE, 0xD8, 0x51, 0xD9, 0x04, 0xAE,
    0xD8, 0x79, 0xD9, 0x04, 0xD8, 0x81, 0xF3, 0x9D, 0xAD, 0x00, 0x8D, 0xAE, 0x19, 0x81, 0xAD, 0xD9,
    0x01, 0xD8, 0xF2, 0xAE, 0xDA, 0x26, 0xD8, 0x8E, 0x91, 0x29, 0x83, 0xA7, 0xD9, 0xAD, 0xAD, 0xAD,
    0xAD, 0xF3, 0x2A, 0xD8, 0xD8, 0xF1, 0xB0, 0xAC, 0x89, 0x91, 0x3E, 0x5E, 0x76, 0xF3, 0xAC, 0x2E,
    0x2E, 0xF1, 0xB1, 0x8C, 0x5A, 0x9C, 0xAC, 0x2C, 0x28, 0x28, 0x28, 0x9C, 0xAC, 0x30, 0x18, 0xA8,
    0x98, 0x81, 0x28, 0x34, 0x3C, 0x97, 0x24, 0xA7, 0x28, 0x34, 0x3C, 0x9C, 0x24, 0xF2, 0xB0, 0x89,
    0xAC, 0x91, 0x2C, 0x4C, 0x6C, 0x8A, 0x9B, 0x2D, 0xD9, 0xD8, 0xD8, 0x51, 0xD9, 0xD8, 0xD8, 0x79,

    // bank 7, 138 bytes (remainder)
    0xD9, 0xD8, 0xD8, 0xF1, 0x9E, 0x88, 0xA3, 0x31, 0xDA, 0xD8, 0xD8, 0x91, 0x2D, 0xD9, 0x28, 0xD8,
    0x4D, 0xD9, 0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x83, 0x93, 0x35, 0x3D, 0x80, 0x25, 0xDA,
    0xD8, 0xD8, 0x85, 0x69, 0xDA, 0xD8, 0xD8, 0xB4, 0x93, 0x81, 0xA3, 0x28, 0x34, 0x3C, 0xF3, 0xAB,
    0x8B, 0xF8, 0xA3, 0x91, 0xB6, 0x09, 0xB4, 0xD9, 0xAB, 0xDE, 0xFA, 0xB0, 0x87, 0x9C, 0xB9, 0xA3,
    0xDD, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x95, 0xF1, 0xA3, 0xA3, 0xA3, 0x9D, 0xF1, 0xA3, 0xA3, 0xA3,
    0xA3, 0xF2, 0xA3, 0xB4, 0x90, 0x80, 0xF2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3,
    0xA3, 0xB2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xB0, 0x87, 0xB5, 0x99, 0xF1, 0xA3, 0xA3, 0xA3,
    0x98, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x97, 0xA3, 0xA3, 0xA3, 0xA3, 0xF3, 0x9B, 0xA3, 0xA3, 0xDC,
    0xB9, 0xA7, 0xF1, 0x26, 0x26, 0x26, 0xD8, 0xD8, 0xFF};

const unsigned char dmpConfig[MPU6050_DMP_CONFIG_SIZE] = {
    //  BANK    OFFSET  LENGTH  [DATA]
    0x03, 0x7B, 0x03, 0x4C, 0xCD, 0x6C,                   // FCFG_1 inv_set_gyro_calibration
    0x03, 0xAB, 0x03, 0x36, 0x56, 0x76,                   // FCFG_3 inv_set_gyro_calibration
    0x00, 0x68, 0x04, 0x02, 0xCB, 0x47, 0xA2,             // D_0_104 inv_set_gyro_calibration
    0x02, 0x18, 0x04, 0x00, 0x05, 0x8B, 0xC1,             // D_0_24 inv_set_gyro_calibration
    0x01, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00,             // D_1_152 inv_set_accel_calibration
    0x03, 0x7F, 0x06, 0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97, // FCFG_2 inv_set_accel_calibration
    0x03, 0x89, 0x03, 0x26, 0x46, 0x66,                   // FCFG_7 inv_set_accel_calibration
    0x00, 0x6C, 0x02, 0x20, 0x00,                         // D_0_108 inv_set_accel_calibration
    0x02, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_00 inv_set_compass_calibration
    0x02, 0x44, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_01
    0x02, 0x48, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_02
    0x02, 0x4C, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_10
    0x02, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_11
    0x02, 0x54, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_12
    0x02, 0x58, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_20
    0x02, 0x5C, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_21
    0x02, 0xBC, 0x04, 0x00, 0x00, 0x00, 0x00,             // CPASS_MTX_22
    0x01, 0xEC, 0x04, 0x00, 0x00, 0x40, 0x00,             // D_1_236 inv_apply_endian_accel
    0x03, 0x7F, 0x06, 0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97, // FCFG_2 inv_set_mpu_sensors
    0x04, 0x02, 0x03, 0x0D, 0x35, 0x5D,                   // CFG_MOTION_BIAS inv_turn_on_bias_from_no_motion
    0x04, 0x09, 0x04, 0x87, 0x2D, 0x35, 0x3D,             // FCFG_5 inv_set_bias_update
    0x00, 0xA3, 0x01, 0x00,                               // D_0_163 inv_set_dead_zone
    // SPECIAL 0x01 = enable interrupts
    0x00, 0x00, 0x00, 0x01,                         // SET INT_ENABLE at i=22, SPECIAL INSTRUCTION
    0x07, 0x86, 0x01, 0xFE,                         // CFG_6 inv_set_fifo_interupt
    0x07, 0x41, 0x05, 0xF1, 0x20, 0x28, 0x30, 0x38, // CFG_8 inv_send_quaternion
    0x07, 0x7E, 0x01, 0x30,                         // CFG_16 inv_set_footer
    0x07, 0x46, 0x01, 0x9A,                         // CFG_GYRO_SOURCE inv_send_gyro
    0x07, 0x47, 0x04, 0xF1, 0x28, 0x30, 0x38,       // CFG_9 inv_send_gyro -> inv_construct3_fifo
    0x07, 0x6C, 0x04, 0xF1, 0x28, 0x30, 0x38,       // CFG_12 inv_send_accel -> inv_construct3_fifo
    0x02, 0x16, 0x02, 0x00, 0x09                    // D_0_22 inv_set_fifo_rate

    // This very last 0x01 WAS a 0x09, which drops the FIFO rate down to 20 Hz. 0x07 is 25 Hz,
    // 0x01 is 100Hz. Going faster than 100Hz (0x00=200Hz) tends to result in very noisy data.
    // DMP output frequency is calculated easily using this equation: (200Hz / (1 + value))

    // It is important to make sure the host processor can keep up with reading and processing
    // the FIFO output at the desired rate. Handling FIFO overflow cleanly is also a good idea.
};

const unsigned char dmpUpdates[MPU6050_DMP_UPDATES_SIZE] = {
    0x01, 0xB2, 0x02, 0xFF, 0xFF,
    0x01, 0x90, 0x04, 0x09, 0x23, 0xA1, 0x35,
    0x01, 0x6A, 0x02, 0x06, 0x00,
    0x01, 0x60, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0x04, 0x40, 0x00, 0x00, 0x00,
    0x01, 0x62, 0x02, 0x00, 0x00,
    0x00, 0x60, 0x04, 0x00, 0x40, 0x00, 0x00};

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
struct Quaternion q;        // [w, x, y, z]         quaternion container
struct VectorInt16 aa;      // [x, y, z]            accel sensor measurements
struct VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
struct VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
struct VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3];             // [psi, theta, phi]    Euler angle container
float ypr[3];               // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
volatile unsigned long timez;

void dmpDataReady()
{
    mpuInterrupt = true;
}

PWM_Module Left_Motor;
PWM_Module Right_Motor;

void PWM_Module_Initialize(PWM_Module *left_motor, PWM_Module *right_motor)
{
    ANSBbits.ANSB0 = 0;
    ANSBbits.ANSB1 = 0;
    ANSBbits.ANSB14 = 0;
    ANSBbits.ANSB15 = 0;

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;

    left_motor->Initialize = PWM_OC1_Initialize;
    left_motor->GetDutyCycle = PWM_Get_OC1_DutyCycle;
    left_motor->GetFrequency = PWM_Get_OC1_Frequency;
    left_motor->UpdateDutyCycle = PWM_Update_OC1_DutyCycle;
    left_motor->UpdateFrequency = PWM_Update_OC1_Frequency;

    right_motor->Initialize = PWM_OC2_Initialize;
    right_motor->GetDutyCycle = PWM_Get_OC2_DutyCycle;
    right_motor->GetFrequency = PWM_Get_OC2_Frequency;
    right_motor->UpdateDutyCycle = PWM_Update_OC2_DutyCycle;
    right_motor->UpdateFrequency = PWM_Update_OC2_Frequency;
}

void MPU6050_setup()
{
    printf("Initializing I2C devices...\n");
    MPU6050_address(MPU6050_I2C_ADDRESS);

    MPU6050_initialize();

    // verify connection
    printf("Testing device connections...\n");
    if (MPU6050_testConnection())
        printf("MPU6050 connection successful\n");
    else
        printf("MPU6050 connection failed\n");

    // wait for ready
    printf("\nSend any character to begin DMP programming and demo: \n");

    // load and configure the DMP
    printf("Initializing DMP...\n");
    devStatus = MPU6050_dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    //    MPU6050_setXGyroOffset(220);
    //    MPU6050_setYGyroOffset(76);
    //    MPU6050_setZGyroOffset(-85);
    //    MPU6050_setZAccelOffset(1788); // 1688 factory default for my test chip

    
    MPU6050_setXAccelOffset(-4733);
    MPU6050_setYAccelOffset(928);
    MPU6050_setZAccelOffset(990);
    MPU6050_setXGyroOffset(-614);
    MPU6050_setYGyroOffset(-16);
    MPU6050_setZGyroOffset(-1);
    //

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        printf("Enabling DMP...\n");
        MPU6050_setDMPEnabled(true);

        printf("Enabling interrupt detection (external interrupt B14 pin)...\n");
        IC1_Initialize();
        mpuIntStatus = MPU6050_getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready! Waiting for first interrupt...\n");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = MPU6050_dmpGetFIFOPacketSize();
        printf("packetSize: %d\n", packetSize);
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}

uint8_t MPU6050_dmpInitialize()
{
    // reset device
    printf("\n Resetting MPU6050...\n");
    MPU6050_reset();
    __delay_ms(30); // wait after reset

    // disable sleep mode
    printf("Disabling sleep mode...\n");
    MPU6050_setSleepEnabled(false);

    // get MPU hardware revision
    printf("Selecting user bank 16...\n");
    MPU6050_setMemoryBank(0x10, true, true);
    printf("Selecting memory byte 6...\n");
    MPU6050_setMemoryStartAddress(0x06);
    printf("Checking hardware revision...\n");
    uint8_t hwRevision = MPU6050_readMemoryByte();
    printf("Revision @ user[16][6] = %x\n", hwRevision);
    printf("Resetting memory bank selection to 0...\n");
    MPU6050_setMemoryBank(0, false, false);

    // check OTP bank valid
    printf("Reading OTP bank valid flag...\n");
    uint8_t otpValid = MPU6050_getOTPBankValid();
    printf("OTP bank is ");
    (otpValid ? printf("valid!\n") : printf("invalid!\n"));

    // get X/Y/Z gyro offsets
    printf("Reading gyro offset TC values...\n");
    int8_t xgOffsetTC = MPU6050_getXGyroOffsetTC();
    int8_t ygOffsetTC = MPU6050_getYGyroOffsetTC();
    int8_t zgOffsetTC = MPU6050_getZGyroOffsetTC();
    printf("X gyro offset = %d\n", xgOffsetTC);
    printf("Y gyro offset = %d\n", ygOffsetTC);
    printf("Z gyro offset = %d\n", zgOffsetTC);

    // setup weird slave stuff (?)
    printf("Setting slave 0 address to 0x7F...\n");
    MPU6050_setSlaveAddress(0, 0x7F);
    printf("Disabling I2C Master mode...\n");
    MPU6050_setI2CMasterModeEnabled(false);
    printf("Setting slave 0 address to 0x68 (self)...\n");
    MPU6050_setSlaveAddress(0, 0x68);
    printf("Resetting I2C Master control...\n");
    MPU6050_resetI2CMaster();
    __delay_ms(20);

    // load DMP code into memory banks
    printf("Writing DMP code to MPU memory banks %d bytes\n", MPU6050_DMP_CODE_SIZE);

    if (MPU6050_writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE, 0, 0, 0))
    {
        printf("Success! DMP code written and verified\n");

        // write DMP configuration
        printf("Writing DMP configuration to MPU memory banks ( %d bytes in config def)\n", MPU6050_DMP_CONFIG_SIZE);

        if (MPU6050_writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE))
        {
            printf("Success! DMP configuration written and verified...\n");

            printf("Setting clock source to Z Gyro...\n");
            MPU6050_setClockSource(MPU6050_CLOCK_PLL_ZGYRO);

            printf("Setting DMP and FIFO_OFLOW interrupts enabled...\n");
            MPU6050_setIntEnabled(0x12);

            printf("Setting sample rate to 200Hz...\n");
            MPU6050_setRate(4); // 1khz / (1 + 4) = 200 Hz

            printf("Setting external frame sync to TEMP_OUT_L[0]...\n");
            MPU6050_setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);

            printf("Setting DLPF bandwidth to 42Hz...\n");
            MPU6050_setDLPFMode(MPU6050_DLPF_BW_42);

            printf("Setting gyro sensitivity to +/- 2000 deg/sec...\n");
            MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

            printf("Setting DMP configuration bytes (function unknown)...\n");
            MPU6050_setDMPConfig1(0x03);
            MPU6050_setDMPConfig2(0x00);

            printf("Clearing OTP Bank flag...\n");
            MPU6050_setOTPBankValid(false);

            printf("Setting X/Y/Z gyro offset TCs to previous values...\n");
            MPU6050_setXGyroOffsetTC(xgOffsetTC);
            MPU6050_setYGyroOffsetTC(ygOffsetTC);
            MPU6050_setZGyroOffsetTC(zgOffsetTC);

            printf("Writing final memory update 1/7 (function unknown)...\n");
            uint8_t dmpUpdate[16], j;
            uint16_t pos = 0;
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], 1, true);

            printf("Writing final memory update 2/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], 1, true);

            printf("Resetting FIFO...\n");
            MPU6050_resetFIFO();

            printf("Reading FIFO count...\n");
            uint16_t fifoCount = MPU6050_getFIFOCount();
            uint8_t fifoBuffer[64];
            //
            printf("Current FIFO count=%d\n", fifoCount);
            MPU6050_getFIFOBytes(fifoBuffer, fifoCount);

            printf("Setting motion detection threshold to 2...\n");
            MPU6050_setMotionDetectionThreshold(2);

            printf("Setting zero-motion detection threshold to 156...\n");
            MPU6050_setZeroMotionDetectionThreshold(156);

            printf("Setting motion detection duration to 80...\n");
            MPU6050_setMotionDetectionDuration(80);

            printf("Setting zero-motion detection duration to 0...\n");
            MPU6050_setZeroMotionDetectionDuration(0);

            printf("Resetting FIFO...\n");
            MPU6050_resetFIFO();

            printf("Enabling FIFO...\n");
            MPU6050_setFIFOEnabled(true);

            printf("Enabling DMP...\n");
            MPU6050_setDMPEnabled(true);

            printf("Resetting DMP...\n");
            MPU6050_resetDMP();

            printf("Writing final memory update 3/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], 1, true);

            printf("Writing final memory update 4/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], 1, true);

            printf("Writing final memory update 5/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], 1, true);

            printf("Waiting for FIFO count > 2...\n");
            while ((fifoCount = MPU6050_getFIFOCount()) < 3)
                ;

            printf("Current FIFO count= %d\n", fifoCount);

            printf("Reading FIFO data...\n");
            MPU6050_getFIFOBytes(fifoBuffer, fifoCount);

            printf("Reading interrupt status...\n");
            uint8_t mpuIntStatus = MPU6050_getIntStatus();

            printf("Current interrupt status=%x\n", mpuIntStatus);

            printf("Reading final memory update 6/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            printf("Waiting for FIFO count > 2...\n");
            while ((fifoCount = MPU6050_getFIFOCount()) < 3)
                ;

            printf("Current FIFO count= %d\n", fifoCount);

            printf("Reading FIFO data...\n");
            MPU6050_getFIFOBytes(fifoBuffer, fifoCount);

            printf("Reading interrupt status...\n");
            mpuIntStatus = MPU6050_getIntStatus();

            printf("Current interrupt status=%x\n", mpuIntStatus);

            printf("Writing final memory update 7/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
                dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU6050_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], true, false);
            printf("DMP is good to go! Finally....\n");

            printf("Disabling DMP (you turn it on later)...\n");
            MPU6050_setDMPEnabled(false);

            printf("Setting up internal 42-byte (default) DMP packet buffer...\n");
            dmpPacketSize = 42;

            printf("Resetting FIFO and clearing INT status one last time...\n");
            MPU6050_resetFIFO();
            MPU6050_getIntStatus();
        }
        else
        {
            printf("ERROR! DMP configuration verification failed.....\n");
            return 2; // configuration block loading failed
        }
    }
    else
    {
        printf("ERROR! DMP code verification failed...\n");
        return 1; // main binary block loading failed
    }
    return 0; // success
}

uint16_t MPU6050_dmpGetFIFOPacketSize()
{
    return dmpPacketSize;
}

uint8_t MPU6050_dmpGetYawPitchRoll(float *data, struct Quaternion *q, struct VectorFloat *gravity)
{
    // yaw: (about Z axis)
    data[0] = atan2(2 * q->x * q->y - 2 * q->w * q->z, 2 * q->w * q->w + 2 * q->x * q->x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(gravity->x / sqrt(gravity->y * gravity->y + gravity->z * gravity->z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(gravity->y / sqrt(gravity->x * gravity->x + gravity->z * gravity->z));
    return 0;
}

uint8_t MPU6050_dmpGetQuaternion(struct Quaternion *q, const uint8_t *packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16_t qI[4];
    uint8_t status = MPU6050_dmpGetQuaternion_integer(qI, packet);
    if (status == 0)
    {
        q->w = (float)qI[0] / 16384.0f;
        q->x = (float)qI[1] / 16384.0f;
        q->y = (float)qI[2] / 16384.0f;
        q->z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}

uint8_t MPU6050_dmpGetQuaternion_integer(int16_t *data, const uint8_t *packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0)
        packet = dmpPacketBuffer;
    data[0] = ((packet[0] << 8) + packet[1]);
    data[1] = ((packet[4] << 8) + packet[5]);
    data[2] = ((packet[8] << 8) + packet[9]);
    data[3] = ((packet[12] << 8) + packet[13]);
    return 0;
}

uint8_t MPU6050_dmpGetGravity(struct VectorFloat *v, struct Quaternion *q)
{
    v->x = 2 * (q->x * q->z - q->w * q->y);
    v->y = 2 * (q->w * q->x + q->y * q->z);
    v->z = q->w * q->w - q->x * q->x - q->y * q->y + q->z * q->z;
    return 0;
}

void CompileUserCommands(uint8_t CMDBuff[], uint8_t Command, bool *Moving)
{
    if (*Moving == 0)
    {
        // When static build up an array of commands.  !st byte is # commands
        // GO btn executes or can CANCEL commands.  When we get a go then start moving
        if (Command >= CANCEL)
        {
            CMDBuff[0]++;
            CMDBuff[CMDBuff[0]] = Command;

            if (Command == GO)
            {
                *Moving = 1;
            }

            if (Command == CANCEL)
            {
                CMDBuff[0] = 0;
            }
        } //END IF
    }     //END IF
} //END CompileUserCommands

void PullNextUserCommand(uint8_t CMDBuff[], uint8_t Moving, uint8_t *CurrentCMD) // @SM if moving then execute next command
{
    static uint8_t ptr;

    if (Moving)
    {
        if ((*CurrentCMD == NONE) || (*CurrentCMD == CRASH))
        {
            //need a new CMD...
            if (ptr <= CMDBuff[0])
            {

                //CMDs still in there
                *CurrentCMD = CMDBuff[ptr];
                CMDBuff[ptr] = 0; // clear to make debugging easier
                ptr++;
            }
            else
                CMDBuff[0] = 0; //Empty buffer
        }
    } //END IF Moving
    else
    {
        ptr = 1; //set ptr to 1st cmd
    }

} //END PullNextUserCommand

void LimitInt(int *x, int Min, int Max)
{
    if (*x > Max)
        *x = Max;
    if (*x < Min)
        *x = Min;

} //END LimitInt

//
// Clamp a float between a min and max.  Note doubles are the same
// as floats on this platform.

void LimitFloat(float *x, float Min, float Max)
{
    if (*x > Max)
        *x = Max;
    if (*x < Min)
        *x = Min;

} //END LimitFloat

void GetHeading(float *Heading, float *HeadingTgt, bool Moving)
{
    //calc heading from IMU
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = MPU6050_getIntStatus();

    // get current FIFO count
    fifoCount = MPU6050_getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // reset so we can continue cleanly
        MPU6050_resetFIFO();
        // printf("FIFO overflow!\n");

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize)
            fifoCount = MPU6050_getFIFOCount();

        // read a packet from FIFO
        MPU6050_getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // display Euler angles in degrees
        MPU6050_dmpGetQuaternion(&q, fifoBuffer);
        MPU6050_dmpGetGravity(&gravity, &q);
        MPU6050_dmpGetYawPitchRoll(ypr, &q, &gravity);
        //Serial.print("ypr\t");
        *Heading = (ypr[0] * 180 / M_PI) + 180;
        //        printf("%f\n", (ypr[0] * 180 / M_PI) + 180);

    } //done

    if (!Moving)
    {
        *HeadingTgt = *Heading;
    }
} //END GetHeading

int normalise(const int value, const int start, const int end)
{
    const int width = end - start;         //
    const int offsetValue = value - start; // value relative to 0

    return (offsetValue - ((offsetValue / width) * width)) + start;
    // + start to reset back to start of original range
}

void DriveMotors(int PDrive, int SDrive, bool Moving)
{

    int Mag;

    if (!Moving)
    {
        PDrive = 0;
        SDrive = 0;
    }
    int leftSpeed, rightSpeed;

    LimitInt(&PDrive, -1000, 1000);
    LimitInt(&SDrive, -1000, 1000);

    // ========= Port drive =========
    Mag = abs(PDrive) * .205 + 25;
    if (PDrive == 0)
        Mag = 0;

    if (PDrive < 0)
    {
        LATBbits.LATB15 = 0;
        leftSpeed = normalise(255 - Mag, 0, 50);
        Left_Motor.dutyCyclePercentage = leftSpeed;
        Left_Motor.UpdateDutyCycle(&Left_Motor);
    }
    else
    {
        LATBbits.LATB15 = 1;
        leftSpeed = normalise(Mag, 0, 50);
        Left_Motor.dutyCyclePercentage = leftSpeed;
        Left_Motor.UpdateDutyCycle(&Left_Motor);
    }

    // ========= Stbd drive =========
    Mag = abs(SDrive) * .205 + 25;
    if (SDrive == 0)
        Mag = 0;

    if (SDrive < 0)
    {
        LATBbits.LATB14 = 0;
        rightSpeed = normalise(255 - Mag, 0, 50);
        Right_Motor.dutyCyclePercentage = rightSpeed;
        Right_Motor.UpdateDutyCycle(&Right_Motor);
    }
    else
    {
        LATBbits.LATB14 = 1;
        rightSpeed = normalise(Mag, 0, 50);
        Right_Motor.dutyCyclePercentage = rightSpeed;
        Right_Motor.UpdateDutyCycle(&Right_Motor);
    }

} //END DriveMotors

void PID(float Hdg, float HdgTgt, int *Demand, float kP, float kI, float kD, bool Moving)
{
    static unsigned long lastTime;
    static float Output;
    static float errSum, lastErr, error;

    // IF not moving then
    if (!Moving)
    {
        errSum = 0;
        lastErr = 0;
        return;
    }

    //error correction for angular overlap
    error = Hdg - HdgTgt;
    if (error < 180)
        error += 360;
    if (error > 180)
        error -= 360;

    //http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

    /*How long since we last calculated*/
    unsigned long now = timez;
    float timeChange = (float)(now - lastTime);
    /*Compute all the working error variables*/
    //float error = Setpoint - Input;
    errSum += (error * timeChange);

    //integral windup guard
    LimitFloat(&errSum, -300, 300);

    float dErr = (error - lastErr) / timeChange;

    /*Compute PID Output*/
    *Demand = kP * error + kI * errSum + kD * dErr;
    /*Remember some variables for next time*/
    lastErr = error;
    lastTime = now;

    //limit demand
    LimitInt(Demand, -400, 400);

} //END getPID

void CheckIMU(uint8_t *Command, float Heading)
{
    static int Init = 1, Count;
    static float oHeading;

    if (Init)
    {
        //If IMU not stable don't allow the robot to start navigating.
        if (*Command == GO)
            *Command = NONE;

        Count++;
        if (Count > 2)
        {
            Count = 0;
            if (abs(Heading - oHeading) < 1)
            {
                Init = 0;
                *Command = IMU_WARM;
            }
            else
                oHeading = Heading;
        }
    }
} //END Check IMU

void ExecuteCommand(uint8_t *CurrentCMD, bool *Moving, float *HeadingTgt, float Demand, uint8_t Command, float Heading)
{
    static uint8_t state = 0;
    static int ForeDmd, Time;
    const int ExecuteDelay = 80;
    if (*Moving)
    {
        if (state == 0)
        {

            switch (*CurrentCMD)
            {
            case UP:
                ForeDmd = 500;
                Time = ExecuteDelay * .7;
                state++;
                break;
            case DN:
                ForeDmd = -400;
                Time = ExecuteDelay * .7;
                state++;
                break;
            case LEFT:
                *HeadingTgt -= 90;
                Time = 15;
                state++;
                if (*HeadingTgt < 0)
                    *HeadingTgt += 360;
                break;
            case RIGHT:
                *HeadingTgt += 90;
                Time = 15;
                state++;
                if (*HeadingTgt > 360)
                    *HeadingTgt -= 360;
                break;
            case GO:
                state++;
                Time = ExecuteDelay;

                break;
            case NONE:
                Right_Motor.dutyCyclePercentage = 0;
                Right_Motor.UpdateDutyCycle(&Right_Motor);

                Left_Motor.dutyCyclePercentage = 0;
                Left_Motor.UpdateDutyCycle(&Left_Motor);

                break;
            } //END switch

        } //END IF
        else
        {
            if (Command == CRASH)
                ForeDmd = 0;

            // count down them move back to CMD execution state
            Time--;

            // if (*CurrentCMD == LEFT || *CurrentCMD == RIGHT)
            // {
            //     if (abs(*HeadingTgt - Heading) < 5)
            //     {
            //         printf("Canceled Early\n");
            //         Time = 0;
            //     }
            // }

            if ((Time == 0))
            {
                //we've reached the end of the program!
                if (*CurrentCMD == GO)
                {
                    printf("Go found");
                    *Moving = 0;
                }
                state = 0;
                *CurrentCMD = NONE;
                ForeDmd = 0;
            }
        }
    }
    else
    {
        //idle waiting for go cmd
        state = 0;
        //NOT Moving
        ForeDmd = 0;
    }
    DriveMotors((Demand * -1) + ForeDmd, Demand + ForeDmd, *Moving);
} //END ExecuteCommand

void DecodeUserSwitch(uint8_t *Command, bool Moving, uint8_t *CurrentCMD)
{
    if (Moving)
        return;

    char c;

    *Command = NONE;
    if (UART1_RX_DATA_AVAILABLE)
    {
        c = UART1_Read();
    }

    if (c == 'R' || c == 'r')
        *Command = RIGHT;
    else if (c == 'D' || c == 'd')
        *Command = DN;
    else if (c == 'U' || c == 'u')
        *Command = UP;
    else if (c == 'L' || c == 'l')
        *Command = LEFT;
    else if (c == 'C' || c == 'c')
        *Command = CANCEL;
    else if (c == 'G' || c == 'g')
    {
        *Command = GO;
        *CurrentCMD = NONE; //tells pull to start pulling
    }
    //normal command entry
    if (*Command > NONE)
    {
        printf("Got CMD\n");
    }

} //END DecodeUSerSwitch

int checkCrash()
{
    TMR2 = 0;

    LATBbits.LATB12 = 1;
    __delay_ms(10);
    LATBbits.LATB12 = 0;

    while (!PORTBbits.RB5)
        ;
    T2CONbits.TON = 1;
    while (PORTBbits.RB5)
        ;
    T2CONbits.TON = 0;
    int a = TMR2;

    a = a / 58.82; //Converts Time to Distance
    a = a + 1;     //Distance Calibration

    if (a < 20)
        return -1;

    TMR2 = 0;

    LATBbits.LATB11 = 1;
    __delay_ms(10);
    LATBbits.LATB11 = 0;

    while (!PORTBbits.RB4)
        ;
    T2CONbits.TON = 1;
    while (PORTBbits.RB4)
        ;
    T2CONbits.TON = 0;
    a = TMR2;

    a = a / 58.82; //Converts Time to Distance
    a = a + 1;     //Distance Calibration

    if (a < 20)
        return -1;

    TMR2 = 0;

    LATBbits.LATB10 = 1;
    __delay_ms(10);
    LATBbits.LATB10 = 0;

    while (!PORTBbits.RB3)
        ;
    T2CONbits.TON = 1;
    while (PORTBbits.RB3)
        ;
    T2CONbits.TON = 0;
    a = TMR2;

    a = a / 58.82; //Converts Time to Distance
    a = a + 1;     //Distance Calibration

    if (a < 20)
        return -1;

    return 0;
}

void MPU6050_loop()
{
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB10 = 0;

    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB3 = 1;

    PWM_Module_Initialize(&Left_Motor, &Right_Motor);

    Left_Motor.Initialize(&Left_Motor);
    Right_Motor.Initialize(&Right_Motor);

    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 1;

    //    MPU6050_setFullScaleAccelRange(0);
    //    MPU6050_setFullScaleGyroRange(0);

    Right_Motor.dutyCyclePercentage = 25;
    Right_Motor.UpdateDutyCycle(&Right_Motor);

    Left_Motor.dutyCyclePercentage = 25;
    Left_Motor.UpdateDutyCycle(&Left_Motor);

    while (1)
    {
        static int Demand, SubLoop;
        static float Heading, HeadingTgt;
        static bool Moving = false;
        static uint8_t Command, CurrentCMD, CMDBuff[256];

        float Reduction = 0.3;

        SubLoop++;
        if (!dmpReady)
            return;
        while (!mpuInterrupt && fifoCount < packetSize)
        {
        }

        GetHeading(&Heading, &HeadingTgt, Moving);
        //PID(Hdg, HdgTgt, *Demand, kP, kI, kD, Moving)
        PID(Heading, HeadingTgt, &Demand, Reduction * 15, Reduction * .08, 0, Moving);

        if ((SubLoop % 2) == 0)
        {
            DecodeUserSwitch(&Command, Moving, &CurrentCMD);
            // CheckIMU(&Command, Heading); // Look to see when the IMU has warmed up, issue a CMD when it has otherwise prevent start
            //            if (checkCrash() == -1)
            //            {
            //                Moving = 0;
            //            }
            CompileUserCommands(CMDBuff, Command, &Moving);
            PullNextUserCommand(CMDBuff, Moving, &CurrentCMD);

            ExecuteCommand(&CurrentCMD, &Moving, &HeadingTgt, Demand, Command, Heading);
            // printf("%f\n", Heading);
            printf("Heading %f, Heading Tgt: %f, Command: %i Moving:%i\n", Heading, HeadingTgt, Command, Moving);
        }
    }
}

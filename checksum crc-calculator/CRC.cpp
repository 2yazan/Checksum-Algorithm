#include "CRC.h"


uint64_t  CrcCalculator::calculateCrc64(const QByteArray &data) {
    uint64_t crc_table[256];
    uint64_t crc;

    for (int i = 0; i < 256; i++) {
        crc = i;
        for (int j = 0; j < 8; j++) {
            crc = crc & 1 ? (crc >> 1) ^ 0xC96C5795D7870F42ULL : crc >> 1;
        }
        crc_table[i] = crc;
    }

    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(data.constData());
    size_t len = data.size();
    crc = 0xFFFFFFFFFFFFFFFFULL;
    while (len--) {
        crc = crc_table[(crc ^ *buffer++) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFFFFFFFFFFULL;
}

uint32_t CrcCalculator::calculateCrc32(const QByteArray &data) {
    uint32_t crc_table[256];
    uint32_t crc;

    for (int i = 0; i < 256; i++) {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    }

    const char* buffer = data.constData();
    size_t len = data.size();

    crc = 0xFFFFFFFFUL;

    while (len--)
        crc = crc_table[(crc ^ *buffer++) & 0xFF] ^ (crc >> 8);

    return crc ^ 0xFFFFFFFFUL;
}

uint16_t CrcCalculator::calculateCrc16(const QByteArray &data) {
    uint16_t crc_table[256];
    uint16_t crc;

    for (int i = 0; i < 256; i++) {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xA001U : crc >> 1;
        crc_table[i] = crc;
    }

    const char* buffer = data.constData();
    size_t len = data.size();

    crc = 0xFFFFU;
    while (len--)
        crc = crc_table[(crc ^ *buffer++) & 0xFFU] ^ (crc >> 8);

    return crc;
}

unsigned char CrcCalculator::calculateCrc8(const QByteArray &data) {
    unsigned char crc = 0xFF;
    unsigned int i;

    const char* buffer = data.constData();
    size_t len = data.size();

    while (len--) {
        crc ^= *buffer++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}

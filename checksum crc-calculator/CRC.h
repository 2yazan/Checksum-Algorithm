#ifndef CRC_H
#define CRC_H

#include <QByteArray>
#include <cstdint>

class CrcCalculator {
public:
    uint64_t calculateCrc64(const QByteArray &data);
    uint32_t calculateCrc32(const QByteArray &data);
    uint16_t calculateCrc16(const QByteArray &data);
    unsigned char calculateCrc8(const QByteArray &data);
};

#endif // CRC_H

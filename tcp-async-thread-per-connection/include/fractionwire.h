#pragma once

#include <cstdint> // C std-int
#include <arpa/inet.h> // ntohl

struct FractionWire
{
    uint32_t m_numerator{ };
    uint32_t m_denominator{ };

    void printReadable()
    {
        std::cout << ntohl(m_numerator) << "/" << ntohl(m_denominator) << '\n';
    }
};

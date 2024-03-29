#include "ZipPacket.hpp"

namespace swl
{
    const void* ZipPacket::onSend(uint32_t& size)
    {
        char* outData = new char[getSize()+2];
        size = LZ4_compress_default((const char*)getData(), outData, getSize(), getSize()+2);
        return outData;
    }

    void ZipPacket::onReceive(const void* data, const uint32_t& size)
    {
        char* outData = new char[size*2.25];
        uint32_t outSize = LZ4_decompress_safe((const char*)data, outData, size, size*2.25);
        if (outSize > 0)
            append(outData, outSize);
        //Memory leak
    }
}
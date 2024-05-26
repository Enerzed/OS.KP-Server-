#include "CPacket.hpp"

const void* CPacket::onSend(std::size_t& size)
{
    const void* srcData = getData();
    std::size_t srcSize = getDataSize();
    //return compressTheData(srcData, srcSize, &size); // this is a fake function, of course :)
}

void CPacket::onReceive(const void* data, std::size_t size)
{
    std::size_t dstSize;
    //const void* dstData = uncompressTheData(data, size, &dstSize); // this is a fake function, of course :)
    //append(dstData, dstSize);
}
#include <SFML/Network.hpp>
#include "Encryption.hpp"

class EncryptedPacket : public sf::Packet
{
private:
    //std::vector<char> myBuffer;

    virtual const char* OnSend(std::size_t& size)
    {
        const void* srcData = getData();
        // Гляди что творю
        unsigned char* srcDt = reinterpret_cast<unsigned char*>(const_cast<void*>(srcData));

        std::size_t srcSize = getDataSize();
        unsigned char key[] = "KEY";
        unsigned char* encryptedData = (unsigned char *) malloc(srcSize * sizeof(unsigned char));

        return reinterpret_cast<const char*>(encryptedData);
    }

    virtual void OnReceive(const char* data, std::size_t size)
    {
        std::size_t dstSize;
        const void* dstData;
        append(dstData, dstSize);
    }
};
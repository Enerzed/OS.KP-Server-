#include <SFML/Network.hpp>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>


class CPacket : sf::Packet
{
    virtual const void* onSend(std::size_t& size);
    virtual void onReceive(const void* data, std::size_t size);
};
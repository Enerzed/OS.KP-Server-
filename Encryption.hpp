#pragma once

#include <iostream>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "openssl/rand.h"


class AESEncryption 
{
private:
    const std::string key;
    unsigned char iv[EVP_MAX_IV_LENGTH];
    const EVP_CIPHER* cipher;

public:
    AESEncryption(const std::string& key);
    std::string Encrypt(const std::string& plaintext);
    std::string Decrypt(const std::string& ciphertext, unsigned char* ivv);
    void GenerateRandomIV();
    unsigned char* GetIV();
};
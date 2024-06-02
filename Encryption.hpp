#pragma once

#include <iostream>
#include <vector>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "openssl/rand.h"


class AESEncryption 
{
private:
    const EVP_CIPHER* cipher;
    std::string key = "NO_KEY";
    std::string iv = "NO_IV";
public:
    AESEncryption();
    std::string Encrypt(const std::string&);
    std::string Decrypt(const std::string&, std::string);
    void GenerateRandomIV();
    void GenerateRandomKey();
    std::string GetKey();
    std::string GetIV();
    void SetKey(std::string);
    void SetIV(std::string);
};


class RSAEncryption 
{
private:
    std::string publicKey;
    std::string privateKey;
public:
    RSAEncryption();
    std::string Encrypt(const std::string& plaintext);
    std::string Decrypt(const std::string& ciphertext);
    void GenerateKeys();
    std::string GetPublicKey();
    void SetPublicKey(std::string);
};
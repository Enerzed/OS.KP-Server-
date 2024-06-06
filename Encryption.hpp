// Шифрование с использованием EVP методов для повышения уровня безопасности

#pragma once

#include <iostream>
#include <vector>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>


class AESEncryption 
{
private:
    const EVP_CIPHER* cipher;                       // Вид шифрования
    std::string key = "NO_KEY";                     // Ключ
    std::string iv = "NO_IV";                       // Вектор инициализации
public:
    AESEncryption();                                // Констуктор, здесь подключаем основные методы OpenSSL
    std::string Encrypt(const std::string&);        // Шифрование
    std::string Decrypt(const std::string&);        // Дешифрование
    void GenerateRandomIV();                        // Генерируем случайный вектор инициализации длиной 16 байт методом RAND_bytes
    void GenerateRandomKey();                       // Генерируем случайный ключ длиной 32 байта методом RAND_bytes
    std::string GetKey();                           // Получаем ключ
    std::string GetIV();                            // Получаем вектор инициализации
    void SetKey(std::string);                       // Устанавливаем ключ, если сгенерированный не нужен
    void SetIV(std::string);                        // Устанавливаем вектор инициализации, если сгенерированный не нужен
};


class RSAEncryption 
{
private:
    std::string publicKey = "NO_KEY";               // Публичный ключ
    std::string privateKey = "NO_KEY";              // Приватный ключ
public:
    RSAEncryption();                                // Констуктор, здесь подключаем основные методы OpenSSL
    std::string Encrypt(const std::string&);        // Шифрование
    std::string Decrypt(const std::string&);        // Дешифрование
    void GenerateKeys();                            // Генерация RSA ключа
    std::string GetPublicKey();                     // Получение публичного ключа
    void SetPublicKey(std::string);                 // Установка публичного ключа
};
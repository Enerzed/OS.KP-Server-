#include "Encryption.hpp"

AESEncryption::AESEncryption(const std::string& key) : key(key)
{
    OpenSSL_add_all_algorithms();
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    if (!cipher) {
        throw std::runtime_error("Error: AES-256-CBC cipher not found.");
    }
}

std::string AESEncryption::Encrypt(const std::string& plaintext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), iv);

    int len = 0, ciphertext_len = 0;
    int plaintext_len = plaintext.length();

    std::string ciphertext(plaintext_len + EVP_CIPHER_block_size(cipher), '\0');
    unsigned char* ciphertext_ptr = reinterpret_cast<unsigned char*>(&ciphertext[0]);

    EVP_EncryptUpdate(ctx, ciphertext_ptr, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext_len);
    ciphertext_len += len;

    EVP_EncryptFinal_ex(ctx, ciphertext_ptr + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext.substr(0, ciphertext_len);
}

std::string AESEncryption::Decrypt(const std::string& ciphertext, unsigned char* ivv)
{

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), ivv);

    int len = 0, plaintext_len = 0;
    int ciphertext_len = ciphertext.length();

    std::string plaintext(ciphertext_len, '\0');
    unsigned char* plaintext_ptr = reinterpret_cast<unsigned char*>(&plaintext[0]);

    EVP_DecryptUpdate(ctx, plaintext_ptr, &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext_len);
    plaintext_len += len;

    EVP_DecryptFinal_ex(ctx, plaintext_ptr + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext.substr(0, plaintext_len);
}

void AESEncryption::GenerateRandomIV()
{
    if (RAND_bytes(iv, EVP_CIPHER_iv_length(cipher)) != 1)
        throw std::runtime_error("Error generating random IV");
}

unsigned char* AESEncryption::GetIV()
{
    return iv;
}
#include "Encryption.hpp"

AESEncryption::AESEncryption()
{
    OpenSSL_add_all_algorithms();
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    if (!cipher) 
    {
        throw std::runtime_error("Error: AES-256-CBC cipher not found.");
    }
}

std::string AESEncryption::Encrypt(const std::string& plaintext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), (unsigned char*)iv.c_str());

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

std::string AESEncryption::Decrypt(const std::string& ciphertext, std::string iv)
{

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), (unsigned char*)iv.c_str());

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
    unsigned char newIV[16];
    if (RAND_bytes(newIV, sizeof(newIV)) != 1)
        throw std::runtime_error("Error generating random IV");
    iv = (reinterpret_cast<const char*>(newIV));

    //std::cout << sizeof(newIV) << std::endl;
    //std::cout << "IV - " << newIV << std::endl;
    //std::cout << "IV - " << iv << std::endl;
}

void AESEncryption::GenerateRandomKey()
{
    unsigned char newKey[32];
    if (RAND_bytes(newKey, sizeof(newKey)) != 1)
        throw std::runtime_error("Error generating random key");
    key = (reinterpret_cast<const char*>(newKey));

    //std::cout << sizeof(newKey) << std::endl;
    //std::cout << "KEY - " << newKey << std::endl;
    //std::cout << "KEY - " << key << std::endl;
}

void AESEncryption::SetKey(std::string newKey)
{
    key = newKey;
}

void AESEncryption::SetIV(std::string newIV)
{
    iv = newIV;
}

std::string AESEncryption::GetKey()
{
    return key;
}

std::string AESEncryption::GetIV()
{
    return iv;
}

RSAEncryption::RSAEncryption()
{
    OpenSSL_add_all_algorithms();
}

std::string RSAEncryption::Encrypt(const std::string& plaintext)
{
    // Create RSA context
    RSA* rsa = RSA_new();
    BIO* bio = BIO_new_mem_buf(publicKey.c_str(), -1);
    PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);
    BIO_free(bio);

    // Encrypt plaintext
    int maxLength = RSA_size(rsa);
    std::string ciphertext;
    ciphertext.resize(maxLength);
    int ciphertextLength = RSA_public_encrypt(plaintext.length(), reinterpret_cast<const unsigned char*>(plaintext.c_str()), reinterpret_cast<unsigned char*>(&ciphertext[0]), rsa, RSA_PKCS1_PADDING);
    ciphertext.resize(ciphertextLength);

    RSA_free(rsa);

    return ciphertext;
}

std::string RSAEncryption::Decrypt(const std::string& ciphertext) 
{
    // Create RSA context
    RSA* rsa = RSA_new();
    BIO* bio = BIO_new_mem_buf(privateKey.c_str(), -1);
    PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
    BIO_free(bio);

    // Decrypt ciphertext
    int maxLength = RSA_size(rsa);
    std::string plaintext;
    plaintext.resize(maxLength);
    int plaintextLength = RSA_private_decrypt(ciphertext.length(), reinterpret_cast<const unsigned char*>(ciphertext.c_str()), reinterpret_cast<unsigned char*>(&plaintext[0]), rsa, RSA_PKCS1_PADDING);
    plaintext.resize(plaintextLength);

    RSA_free(rsa);

    return plaintext;
}

void RSAEncryption::GenerateKeys()
{
    // Generate RSA keys
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);

    // Export public key to PEM format
    BIO* publicKeyBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(publicKeyBio, rsa);
    char* publicKeyBuffer;
    long publicKeyLength = BIO_get_mem_data(publicKeyBio, &publicKeyBuffer);
    publicKey = std::string(publicKeyBuffer, publicKeyLength);
    BIO_free(publicKeyBio);

    // Export private key to PEM format
    BIO* privateKeyBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(privateKeyBio, rsa, NULL, NULL, 0, NULL, NULL);
    char* privateKeyBuffer;
    long privateKeyLength = BIO_get_mem_data(privateKeyBio, &privateKeyBuffer);
    privateKey = std::string(privateKeyBuffer, privateKeyLength);
    BIO_free(privateKeyBio);

    RSA_free(rsa);
}

std::string RSAEncryption::GetPublicKey()
{
    return publicKey;
}

void RSAEncryption::SetPublicKey(std::string newPublicKey)
{
    publicKey = newPublicKey;
}
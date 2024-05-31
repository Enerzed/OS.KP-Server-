#pragma execution_character_set("utf-8")
#include "Server.hpp"


Server* ServerPtr = nullptr;
void(SignalHandler(int));


void SignalHandler(int signal)
{
    ServerPtr->AddServerVector(signal);
    ServerPtr->ServerDownHandler();
}

int main()
{
    system("chcp 65001");
    std::cout << std::endl;

    // DEBUG

    //RSAEncryption rsa;
    //rsa.GenerateKeys();
    //std::string plaintext = "Hello, RSA!123123123123123123123";
    //std::string ciphertext = rsa.Encrypt(plaintext);
    //std::cout << "Encrypted text: " << ciphertext << std::endl;
    //std::string decrypted_text = rsa.Decrypt(ciphertext);
    //std::cout << "Decrypted text: " << decrypted_text << std::endl;

    //AESEncryption aes;
    ////aes.SetIV("123");
    ////aes.SetKey("keys");
    //aes.GenerateRandomKey();
    //aes.GenerateRandomIV();
    //ciphertext = aes.Encrypt(plaintext);
    //std::cout << ciphertext.length() << std::endl;
    //std::cout << "Encrypted text: " << ciphertext << std::endl;
    //decrypted_text = aes.Decrypt(ciphertext, aes.GetIV());
    //std::cout << decrypted_text.length() << std::endl;
    //std::cout << "Decrypted text: " << decrypted_text << std::endl;


    Server server;
    ServerPtr = &server;

    signal(SIGABRT, SignalHandler);
    signal(SIGINT, SignalHandler);
    signal(SIGBREAK, SignalHandler);
    signal(SIGTERM, SignalHandler);

    server.Run();

    return 0;
}
#include "crypter.h"
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

Crypter::Crypter() {
}

QByteArray Crypter::crypt(QString textToEncrypt, QString password) {
    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    byte iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    std::string text = textToEncrypt.toStdString();
    std::string cipherText;
    for (int i = 0; i < password.size(); i++)
        key[i % CryptoPP::AES::DEFAULT_KEYLENGTH] += password[i].digitValue();

    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipherText));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>(text.c_str()), text.length());
    stfEncryptor.MessageEnd();

    return QByteArray(cipherText.c_str(), cipherText.length());
}

QString Crypter::decrypt(QByteArray encryptedMessage, QString password) {
    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    byte iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    std::string cipherText(encryptedMessage.constData(), encryptedMessage.size());
    std::string decryptedText;
    for (int i = 0; i < password.size(); i++)
        key[i % CryptoPP::AES::DEFAULT_KEYLENGTH] += password[i].digitValue();

    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedText));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipherText.c_str()), cipherText.size());
    stfDecryptor.MessageEnd();

    return QString::fromStdString(decryptedText);
}

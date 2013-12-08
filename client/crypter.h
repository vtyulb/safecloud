#ifndef CRYPTER_H
#define CRYPTER_H

#include <QString>

class Crypter
{
public:
    Crypter();

    static QByteArray crypt(QString textToEncrypt, QString password);
    static QString decrypt(QByteArray encryptedMessage, QString password);
};

#endif // CRYPTER_H

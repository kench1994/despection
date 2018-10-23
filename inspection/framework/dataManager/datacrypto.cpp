#include "datacrypto.h"
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rc2.h>
#include <QDebug>
#include <openssl/err.h>
#include <stdio.h>

#define CRYPTO_BF_CBC_MODE EVP_bf_cbc()   // Blowfish CBC mode
#define CRYPTO_BF_ECB_MODE EVP_bf_ecb()   // Blowfish ECB mode

#define CRYPTO_DES_CBC_MODE EVP_des_cbc()    // DES CBC mode
#define CRYPTO_DES_ECB_MODE EVP_des_ecb()    // DES ECB mode
#define CRYPTO_DES_EDE_MODE EVP_des_ede()    // DES EDE mode
#define CRYPTO_DES_EDE3_MODE EVP_des_ede3()   // DES EDE3 mode

#define CRYPTO_RC2_CBC_MODE EVP_rc2_cbc()    // RC2 CBC mode
#define CRYPTO_RC2_ECB_MODE EVP_rc2_ecb()    // RC2 ECB mode

#define CRYPTO_AES_CBC_128_MODE EVP_aes_128_cbc()
#define CRYPTO_AES_CBC_256_MODE EVP_aes_256_cbc()

#define CRYPTO_RC4_MODE EVP_rc4()      // RC4 mode
#define CRYPTO_RC4_40_MODE EVP_rc4_40()   // RC4 40 mode

void hexDump1(const char* title, const unsigned char* data, int len)
{
    qDebug() << title;
    char hex[400] = {0};
    char *p = hex;
    for (int i = 0; i < len; i++)
        sprintf(hex + i * 5, "0x%02x ", (char*)*(data + i));

    qDebug() << hex;
}

bool DataCrypto::encrypt(const char *in,
                            const int inLen,
                            unsigned char *out,
                            int *outlen,
                            unsigned char *key,
                            unsigned char *iv,
                            CryptoMODE mode,
                            bool padding)
{
    int buflen, tmplen;
    buflen = tmplen = 0;
    hexDump1("plant text:", (const unsigned char *)in, inLen);
    qDebug() << (char*)key;
    qDebug() << (char*)iv;
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    switch (mode) {
    case BF_CBC_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_BF_CBC_MODE, NULL,
                           key, iv);
        break;
    case BF_ECB_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_BF_ECB_MODE, NULL,
                           key, iv);
        break;
    case DES_CBC_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_DES_CBC_MODE, NULL,
                           key, iv);
        break;
    case DES_ECB_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_DES_ECB_MODE, NULL,
                           key, iv);
        break;
    case DES_EDE_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_DES_EDE_MODE, NULL,
                           key, iv);
        break;
    case DES_EDE3_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_DES_EDE3_MODE, NULL,
                           key, iv);
        break;
    case RC2_CBC_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_RC2_CBC_MODE, NULL,
                           key, iv);
        break;
    case RC2_ECB_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_RC2_ECB_MODE, NULL,
                           key, iv);
        break;
    case RC4_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_RC4_MODE, NULL,
                           key, iv);
        break;
    case RC4_40_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_RC4_40_MODE, NULL,
                           key, iv);
        break;
    case AES_CBC_128_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_AES_CBC_128_MODE, NULL,
                           key, iv);
        break;
    case AES_CBC_256_MODE:
        EVP_EncryptInit_ex(&ctx, CRYPTO_AES_CBC_256_MODE, NULL,
                           key, iv);
        break;
    default:
        return false;
    }

    if(!padding){
        if(!EVP_CIPHER_CTX_set_padding(&ctx, 0)){
            return false;
        }
    }

    if(!EVP_EncryptUpdate(&ctx, out, &buflen, (unsigned char*)in, inLen))
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        return false;
    }

    if(!EVP_EncryptFinal_ex(&ctx, out + buflen, &tmplen))
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        return false;
    }

    buflen += tmplen;
    *outlen = buflen;
    EVP_CIPHER_CTX_cleanup(&ctx);
    hexDump1("enc text:", (const unsigned char *)out, *outlen);
    return true;
}

bool DataCrypto::decrypt(const unsigned char *in,
                            unsigned char *out,
                            int inLen,
                            int& outLen,
                            unsigned char *key,
                            unsigned char *iv,
                            CryptoMODE mode,
                            bool padding)
{
    int buflen, tmplen;
    buflen = tmplen = 0;

    hexDump1("enc text:", (const unsigned char *)in, inLen);

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    switch (mode) {
    case BF_CBC_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_BF_CBC_MODE, NULL,
                           key, iv);
        break;
    case BF_ECB_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_BF_ECB_MODE, NULL,
                           key, iv);
        break;
    case DES_CBC_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_DES_CBC_MODE, NULL,
                           key, iv);
        break;
    case DES_ECB_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_DES_ECB_MODE, NULL,
                           key, iv);
        break;
    case DES_EDE_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_DES_EDE_MODE, NULL,
                           key, iv);
        break;
    case DES_EDE3_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_DES_EDE3_MODE, NULL,
                           key, iv);
        break;
    case RC2_CBC_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_RC2_CBC_MODE, NULL,
                           key, iv);
        break;
    case RC2_ECB_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_RC2_ECB_MODE, NULL,
                           key, iv);
        break;
    case RC4_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_RC4_MODE, NULL,
                           key, iv);
        break;
    case RC4_40_MODE:
        EVP_DecryptInit_ex(&ctx, CRYPTO_RC4_40_MODE, NULL,
                           key, iv);
        break;
    case AES_CBC_128_MODE:
        qDebug() << "AES_CBC_128_MODE  EVP_DecryptInit_ex ";
        EVP_DecryptInit_ex(&ctx, CRYPTO_AES_CBC_128_MODE, NULL,
                           key, iv);
        break;
    case AES_CBC_256_MODE:
        qDebug() << "AES_CBC_256_MODE  EVP_DecryptInit_ex ";
        EVP_DecryptInit_ex(&ctx, CRYPTO_AES_CBC_256_MODE, NULL,
                           key, iv);
        break;
    default:
        return false;
    }

    if(!padding){
        if(!EVP_CIPHER_CTX_set_padding(&ctx, 0)){
            return false;
        }
    }

    if(!EVP_DecryptUpdate(&ctx, out, &buflen, in, inLen))
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        return false;
    }

    if(!EVP_DecryptFinal_ex(&ctx, out + buflen, &tmplen))
    {
        unsigned long errcode = ERR_get_error();
        char * errdesc = ERR_error_string(errcode, NULL);
        qDebug() << errdesc;
        EVP_CIPHER_CTX_cleanup(&ctx);
        return false;
    }

    outLen = buflen + tmplen;

    EVP_CIPHER_CTX_cleanup(&ctx);

    hexDump1("dec text:", (const unsigned char *)out, outLen);

    return true;
}

bool DataCrypto::do_encrypt(const char *in, unsigned char *out, int *outlen, unsigned char *key, unsigned char *iv)
{
  int buflen, tmplen;

  EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, CRYPTO_RC2_CBC_MODE, NULL, key, iv);

  if(!EVP_EncryptUpdate(&ctx, out, &buflen, (unsigned char*)in, strlen(in)))
    {
        return false;
  }

  if(!EVP_EncryptFinal_ex(&ctx, out + buflen, &tmplen))
    {
        return false;
  }

  buflen += tmplen;
  *outlen = buflen;
    EVP_CIPHER_CTX_cleanup(&ctx);

  return true;
}

bool DataCrypto::do_decrypt(const unsigned char *in, unsigned char *out, int inlen, unsigned char *key, unsigned char *iv)
{
  int buflen, tmplen;

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  EVP_DecryptInit_ex(&ctx, CRYPTO_RC2_CBC_MODE, NULL, key, iv);

  if(!EVP_DecryptUpdate(&ctx, out, &buflen, in, inlen))
  {
    return false;
  }

  if(!EVP_DecryptFinal_ex(&ctx, out + buflen, &tmplen))
    {
        return false;
  }

  int decryptedLength = buflen + tmplen;
    out[decryptedLength] = '\0';

  EVP_CIPHER_CTX_cleanup(&ctx);

  return true;
}



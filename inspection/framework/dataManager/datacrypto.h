#ifndef DATACRYPTO_H
#define DATACRYPTO_H
#include <QByteArray>

/*!
  \enum CryptoMODE
  CryptoMODE枚举了数据加解密算法支持种类
  \BF_CBC_MODE BF的CBC算法
  \BF_ECB_MODE BF的ECB算法
  \DES_CBC_MODE DES的CBC算法
  \DES_ECB_MODE DES的ECB算法
  \DES_EDE_MODE DES的EDE算法
  \DES_EDE3_MODE DES的EDE3算法
  \RC2_CBC_MODE RC2的CBC算法
  \RC2_ECB_MODE RC2的ECB算法
  \RC4_MODE RC4算法
  \RC4_40_MODE RC40算法
*/
typedef enum __CryptoMODE__ {
    BF_CBC_MODE,
    BF_ECB_MODE,
    DES_CBC_MODE,
    DES_ECB_MODE,
    DES_EDE_MODE,
    DES_EDE3_MODE,
    RC2_CBC_MODE,
    RC2_ECB_MODE,
    RC4_MODE,
    RC4_40_MODE,
    AES_CBC_128_MODE,
    AES_CBC_256_MODE
}CryptoMODE;

/*!
  \class DataCrypto
  \inModule 数据处理模块
  \brief 数据加解密模块
  \ingroup dataManager
  DataCrypto类为上层提供了加解密的接口
*/
class DataCrypto
{
public:
   /**
     * @brief encrypt:压缩数据
     * @param in    : const char* 原始数据流
     * @param inLen : const int 原始数据长度
     * @param out    : unsigned char* 加密后的数据流
     * @param outlen : int* 加密结果长度
     * @param key  : 加密密钥
     * @param iv  : 加密向量
     * @param mode  : 加密算法
     * @return  bool, true 成功, false 失败
     */
    static bool encrypt(const char *in, const int inLen, unsigned char *out, int *outlen, unsigned char *key, unsigned char *iv, CryptoMODE mode, bool padding = true);

    /**
     * @brief decrypt:解压缩数据
     * @param in    : const unsigned char* 原始数据流
     * @param out    : unsigned char* 解密后的数据流
     * @param inLen    : int 原始数据流长度
     * @param outlen : int& 解密后数据长度
     * @param key  : 加密密钥
     * @param iv  : 加密向量
     * @param mode  : 加密算法
     * @return  bool, true 成功, false 失败
     */
    static bool decrypt(const unsigned char *in, unsigned char *out, int inLen, int& outLen, unsigned char *key, unsigned char *iv, CryptoMODE mode, bool padding = true);

    static bool do_encrypt(const char *in, unsigned char *out, int *outlen, unsigned char *key, unsigned char *iv);
    static bool do_decrypt(const unsigned char *in, unsigned char *out, int inlen, unsigned char *key, unsigned char *iv);
};

#endif // DATACRYPTO_H

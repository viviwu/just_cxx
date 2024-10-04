#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

class CryptoUtils {
 public:
  static std::string xor_encrypt_decrypt(const std::string &input,
                                         const std::string &key);

  /***************************************************************/
  /**
   *  凯撒密码 (Caesar Cipher)
   * 凯撒密码是一种替换加密算法，通过将字母表中的每个字母移动固定数量的位置来加密消息。
   */
  static std::string caesar_encrypt(const std::string &input, int shift);
  static std::string caesar_decrypt(const std::string &input, int shift);

  /***************************************************************/
  /**
   * 移位密码 (Shift Cipher)
   * 移位密码是一种简单的加密方法，通过将字符的ASCII值增加或减少固定值来实现。
   */
  static std::string shift_encrypt(const std::string &input, int shift);
  static std::string shift_decrypt(const std::string &input, int shift);

  /***************************************************************/
  /**
   * 反转字符串 (Reversal Cipher)
   * 反转字符串是一种非常简单的加密方法，通过将字符串倒置来加密消息。
   */
  static std::string reverse_string(const std::string &input);

  /***************************************************************/
  /**
   * 使用Vigenère密码 (Vigenère Cipher)
   * Vigenère密码是一种多表替换密码，它使用一个密钥字来加密和解密。
   */
  static std::string vigenere_encrypt(const std::string &input,
                                      const std::string &key);
  static std::string vigenere_decrypt(const std::string &input,
                                      const std::string &key);

  /***************************************************************/
#ifdef OPEN_SSL
  // AES加密
  static std::string encryptAES(const std::string &plaintext,
                                const std::string &key);

  // AES解密
  static std::string decryptAES(const std::string &ciphertext,
                                const std::string &key);

  // RSA加密
  static std::string encryptRSA(const std::string &plaintext,
                                const std::string &publicKey);

  // RSA解密
  static std::string decryptRSA(const std::string &ciphertext,
                                const std::string &privateKey);

  // ECC签名
  static std::string signECC(const std::string &message,
                             const std::string &privateKey);

  // ECC验证签名
  static bool verifyECC(const std::string &message,
                        const std::string &signature,
                        const std::string &publicKey);

  // MD5哈希
  static std::string hashMD5(const std::string &data);

  // SHA-256哈希
  static std::string hashSHA256(const std::string &data);

  // SHA-3哈希
  static std::string hashSHA3(const std::string &data);

#endif
};

#endif  // CRYPTO_H

#include "crypto_utils.h"

#include <iostream>
#include <vector>

/***************************************************************/

std::string CryptoUtils::xor_encrypt_decrypt(const std::string &input,
                                             const std::string &key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] ^ key[i % key.size()];
  }
  return output;
}

/***************************************************************/
/**
 *  凯撒密码 (Caesar Cipher)
 * 凯撒密码是一种替换加密算法，通过将字母表中的每个字母移动固定数量的位置来加密消息。
 */
std::string CryptoUtils::caesar_encrypt(const std::string &input, int shift) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base = islower(input[i]) ? 'a' : 'A';
      output[i] = (input[i] - base + shift) % 26 + base;
    }
  }
  return output;
}

std::string CryptoUtils::caesar_decrypt(const std::string &input, int shift) {
  return caesar_encrypt(input, 26 - shift);
}

/***************************************************************/
/**
 * 移位密码 (Shift Cipher)
 * 移位密码是一种简单的加密方法，通过将字符的ASCII值增加或减少固定值来实现。
 */

std::string CryptoUtils::shift_encrypt(const std::string &input, int shift) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] + shift;
  }
  return output;
}

std::string CryptoUtils::shift_decrypt(const std::string &input, int shift) {
  return shift_encrypt(input, -shift);
}
/***************************************************************/
/**
 * 反转字符串 (Reversal Cipher)
反转字符串是一种非常简单的加密方法，通过将字符串倒置来加密消息。
 */

std::string CryptoUtils::reverse_string(const std::string &input) {
  std::string output = input;
  std::reverse(output.begin(), output.end());
  return output;
}

/***************************************************************/
/**
 * 使用Vigenère密码 (Vigenère Cipher)
Vigenère密码是一种多表替换密码，它使用一个密钥字来加密和解密。
 */

std::string CryptoUtils::vigenere_encrypt(const std::string &input,
                                          const std::string &key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base = islower(input[i]) ? 'a' : 'A';
      char key_base = islower(key[i % key.size()]) ? 'a' : 'A';
      output[i] =
          (input[i] - base + (key[i % key.size()] - key_base)) % 26 + base;
    }
  }
  return output;
}

std::string CryptoUtils::vigenere_decrypt(const std::string &input,
                                          const std::string &key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base = islower(input[i]) ? 'a' : 'A';
      char key_base = islower(key[i % key.size()]) ? 'a' : 'A';
      output[i] =
          (input[i] - base - (key[i % key.size()] - key_base) + 26) % 26 + base;
    }
  }
  return output;
}

#ifdef OPEN_SSL

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

// 实现AES加密
std::string CryptoUtils::encryptAES(const std::string &plaintext,
                                    const std::string &key) {
  AES_KEY aesKey;
  unsigned char iv[AES_BLOCK_SIZE];  // 初始化向量
  std::fill(std::begin(iv), std::end(iv), 0);

  std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);

  AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(key.data()), 128,
                      &aesKey);
  AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(plaintext.data()),
                  ciphertext.data(), plaintext.size(), &aesKey, iv,
                  AES_ENCRYPT);

  return std::string(ciphertext.begin(), ciphertext.end());
}

// 实现AES解密
std::string CryptoUtils::decryptAES(const std::string &ciphertext,
                                    const std::string &key) {
  AES_KEY aesKey;
  unsigned char iv[AES_BLOCK_SIZE];
  std::fill(std::begin(iv), std::end(iv), 0);

  std::vector<unsigned char> plaintext(ciphertext.size());

  AES_set_decrypt_key(reinterpret_cast<const unsigned char *>(key.data()), 128,
                      &aesKey);
  AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(ciphertext.data()),
                  plaintext.data(), ciphertext.size(), &aesKey, iv,
                  AES_DECRYPT);

  return std::string(plaintext.begin(), plaintext.end());
}

// RSA加密
std::string CryptoUtils::encryptRSA(const std::string &plaintext,
                                    const std::string &publicKey) {
  RSA *rsa = RSA_new();
  BIO *keybio = BIO_new_mem_buf(publicKey.data(), -1);
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);

  std::vector<unsigned char> encrypted(RSA_size(rsa));
  RSA_public_encrypt(plaintext.size(),
                     reinterpret_cast<const unsigned char *>(plaintext.data()),
                     encrypted.data(), rsa, RSA_PKCS1_OAEP_PADDING);

  RSA_free(rsa);
  BIO_free_all(keybio);

  return std::string(encrypted.begin(), encrypted.end());
}

// RSA解密
std::string CryptoUtils::decryptRSA(const std::string &ciphertext,
                                    const std::string &privateKey) {
  RSA *rsa = RSA_new();
  BIO *keybio = BIO_new_mem_buf(privateKey.data(), -1);
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

  std::vector<unsigned char> decrypted(RSA_size(rsa));
  RSA_private_decrypt(
      ciphertext.size(),
      reinterpret_cast<const unsigned char *>(ciphertext.data()),
      decrypted.data(), rsa, RSA_PKCS1_OAEP_PADDING);

  RSA_free(rsa);
  BIO_free_all(keybio);

  return std::string(decrypted.begin(), decrypted.end());
}

// ECC签名（略）

// MD5哈希
std::string CryptoUtils::hashMD5(const std::string &data) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(),
      digest);

  return std::string(reinterpret_cast<char *>(digest), MD5_DIGEST_LENGTH);
}

// SHA-256哈希
std::string CryptoUtils::hashSHA256(const std::string &data) {
  unsigned char digest[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(),
         digest);

  return std::string(reinterpret_cast<char *>(digest), SHA256_DIGEST_LENGTH);
}

// SHA-3哈希
std::string CryptoUtils::hashSHA3(const std::string &data) {
  unsigned char digest[SHA512_DIGEST_LENGTH];  // SHA3-512
  SHA3_512(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(),
           digest);

  return std::string(reinterpret_cast<char *>(digest), SHA512_DIGEST_LENGTH);
}

#endif

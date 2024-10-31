#include <algorithm>
#include <iostream>
#include <string>

#include "crypto_utils.h"

using namespace std;

/***************************************************************/

int main_xor_encrypt_decrypt() {
  std::string key = "mysecretkey";
  std::string plain_text = "Hello, World!main_xor_encrypt_decrypt";

  // 加密
  std::string encrypted_text =
      CryptoUtils::xor_encrypt_decrypt(plain_text, key);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  // 解密
  std::string decrypted_text =
      CryptoUtils::xor_encrypt_decrypt(encrypted_text, key);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/

int main_caesar_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_caesar_encrypt_decrypt";
  int shift = 3;

  std::string encrypted_text = CryptoUtils::caesar_encrypt(plain_text, shift);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text =
      CryptoUtils::caesar_decrypt(encrypted_text, shift);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 移位密码 (Shift Cipher)
移位密码是一种简单的加密方法，通过将字符的ASCII值增加或减少固定值来实现。
 */

int main_shift_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_shift_encrypt_decrypt";
  int shift = 3;

  std::string encrypted_text = CryptoUtils::shift_encrypt(plain_text, shift);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text =
      CryptoUtils::shift_decrypt(encrypted_text, shift);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 反转字符串 (Reversal Cipher)
反转字符串是一种非常简单的加密方法，通过将字符串倒置来加密消息。
 */

int main_reverse_string() {
  std::string plain_text = "Hello, World!main_reverse_string";

  std::string encrypted_text = CryptoUtils::reverse_string(plain_text);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text = CryptoUtils::reverse_string(encrypted_text);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 使用Vigenère密码 (Vigenère Cipher)
Vigenère密码是一种多表替换密码，它使用一个密钥字来加密和解密。
 */

int main_vigenere_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_vigenere_encrypt_decrypt";
  std::string key = "key";

  std::string encrypted_text = CryptoUtils::vigenere_encrypt(plain_text, key);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text =
      CryptoUtils::vigenere_decrypt(encrypted_text, key);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

int main_test() {
  cout << "Hello World!" << endl;
  main_xor_encrypt_decrypt();

  main_caesar_encrypt_decrypt();

  main_shift_encrypt_decrypt();

  main_reverse_string();

  main_vigenere_encrypt_decrypt();

  return 0;
}

int main() {
  main_test();
#ifdef OPEN_SSL
  // 示例调用
  std::string message = "Hello, World!";
  std::string key = "0123456789abcdef";  // 128-bit key for AES
  std::string encrypted = CryptoUtils::encryptAES(message, key);
  std::string decrypted = CryptoUtils::decryptAES(encrypted, key);

  std::cout << "Encrypted: " << encrypted << std::endl;
  std::cout << "Decrypted: " << decrypted << std::endl;

  std::string hashed = CryptoUtils::hashSHA256(message);
  std::cout << "SHA-256 Hash: " << hashed << std::endl;
#endif

  return 0;
}

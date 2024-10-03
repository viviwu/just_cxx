#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

/***************************************************************/

std::string xor_encrypt_decrypt(const std::string& input,
                                const std::string& key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] ^ key[i % key.size()];
  }
  return output;
}

int main_xor_encrypt_decrypt() {
  std::string key        = "mysecretkey";
  std::string plain_text = "Hello, World!main_xor_encrypt_decrypt";

  // 加密
  std::string encrypted_text = xor_encrypt_decrypt(plain_text, key);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  // 解密
  std::string decrypted_text = xor_encrypt_decrypt(encrypted_text, key);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 *  凯撒密码 (Caesar Cipher)
 * 凯撒密码是一种替换加密算法，通过将字母表中的每个字母移动固定数量的位置来加密消息。
*/
std::string caesar_encrypt(const std::string& input, int shift) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base = islower(input[i]) ? 'a' : 'A';
      output[i] = (input[i] - base + shift) % 26 + base;
    }
  }
  return output;
}

std::string caesar_decrypt(const std::string& input, int shift) {
  return caesar_encrypt(input, 26 - shift);
}

int main_caesar_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_caesar_encrypt_decrypt";
  int shift              = 3;

  std::string encrypted_text = caesar_encrypt(plain_text, shift);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text = caesar_decrypt(encrypted_text, shift);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 移位密码 (Shift Cipher)
移位密码是一种简单的加密方法，通过将字符的ASCII值增加或减少固定值来实现。
 */

std::string shift_encrypt(const std::string& input, int shift) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] + shift;
  }
  return output;
}

std::string shift_decrypt(const std::string& input, int shift) {
  return shift_encrypt(input, -shift);
}

int main_shift_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_shift_encrypt_decrypt";
  int shift              = 3;

  std::string encrypted_text = shift_encrypt(plain_text, shift);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text = shift_decrypt(encrypted_text, shift);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 反转字符串 (Reversal Cipher)
反转字符串是一种非常简单的加密方法，通过将字符串倒置来加密消息。
 */

std::string reverse_string(const std::string& input) {
  std::string output = input;
  std::reverse(output.begin(), output.end());
  return output;
}

int main_reverse_string() {
  std::string plain_text = "Hello, World!main_reverse_string";

  std::string encrypted_text = reverse_string(plain_text);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text = reverse_string(encrypted_text);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

/***************************************************************/
/**
 * 使用Vigenère密码 (Vigenère Cipher)
Vigenère密码是一种多表替换密码，它使用一个密钥字来加密和解密。
 */

std::string vigenere_encrypt(const std::string& input, const std::string& key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base     = islower(input[i]) ? 'a' : 'A';
      char key_base = islower(key[i % key.size()]) ? 'a' : 'A';
      output[i] =
          (input[i] - base + (key[i % key.size()] - key_base)) % 26 + base;
    }
  }
  return output;
}

std::string vigenere_decrypt(const std::string& input, const std::string& key) {
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    if (isalpha(input[i])) {
      char base     = islower(input[i]) ? 'a' : 'A';
      char key_base = islower(key[i % key.size()]) ? 'a' : 'A';
      output[i] =
          (input[i] - base - (key[i % key.size()] - key_base) + 26) % 26 + base;
    }
  }
  return output;
}

int main_vigenere_encrypt_decrypt() {
  std::string plain_text = "Hello, World!main_vigenere_encrypt_decrypt";
  std::string key        = "key";

  std::string encrypted_text = vigenere_encrypt(plain_text, key);
  std::cout << "Encrypted: " << encrypted_text << std::endl;

  std::string decrypted_text = vigenere_decrypt(encrypted_text, key);
  std::cout << "Decrypted: " << decrypted_text << std::endl;

  return 0;
}

int main() {
  cout << "Hello World!" << endl;
  main_xor_encrypt_decrypt();

  main_caesar_encrypt_decrypt();

  main_shift_encrypt_decrypt();

  main_reverse_string();

  main_vigenere_encrypt_decrypt();

  return 0;
}

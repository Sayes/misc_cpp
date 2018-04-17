/*
 * g++ crypto_op.cc -std=c++11 -lcrypto -o release/crypto_op
 */

#include <openssl/des.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <winsock.h>
extern "C" {
#include <openssl/applink.c>
}
#else
#include <arpa/inet.h>
#endif

typedef enum {
  GENERAL = 0,
  ECB,
} CRYPTO_MODE;

std::string des_encrypt(const std::string &cleartext, const std::string &key);
std::string des_decrypt(const std::string &ciphertext, const std::string &key);
char *rsa_encrypt(const unsigned char *str, const char *public_key_filename,
                  int &len);
char *rsa_decrypt(const unsigned char *str, const char *private_key_filename,
                  int &len);

std::string des_encrypt(const std::string &cleartext, const std::string &key) {
  std::string strCipherText;
  CRYPTO_MODE mode = GENERAL;

  switch (mode) {
    case GENERAL:
    case ECB: {
      DES_cblock keyEncrypt;
      memset(keyEncrypt, 0, 8);

      if (key.length() <= 8)
        memcpy(keyEncrypt, key.c_str(), key.length());
      else
        memcpy(keyEncrypt, key.c_str(), 8);

      DES_key_schedule keySchedule;
      DES_set_key_unchecked(&keyEncrypt, &keySchedule);

      const_DES_cblock inputText;
      DES_cblock outputText;
      std::vector<unsigned char> vecCiphertext;
      unsigned char tmp[8];

      for (int i = 0; i < cleartext.length() / 8; i++) {
        memcpy(inputText, cleartext.c_str() + i * 8, 8);
        DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
        memcpy(tmp, outputText, 8);

        for (int j = 0; j < 8; j++) vecCiphertext.push_back(tmp[j]);
      }

      if (cleartext.length() % 8 != 0) {
        int tmp1 = cleartext.length() / 8 * 8;
        int tmp2 = cleartext.length() - tmp1;
        memset(inputText, 0, 8);
        memcpy(inputText, cleartext.c_str() + tmp1, tmp2);

        DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
        memcpy(tmp, outputText, 8);

        for (int j = 0; j < 8; j++) vecCiphertext.push_back(tmp[j]);
      }

      strCipherText.clear();
      strCipherText.assign(vecCiphertext.begin(), vecCiphertext.end());
    } break;
  }

  return strCipherText;
}

std::string des_decrypt(const std::string &ciphertext, const std::string &key) {
  std::string strClearText;
  CRYPTO_MODE mode = GENERAL;

  switch (mode) {
    case GENERAL:
    case ECB: {
      DES_cblock keyEncrypt;
      memset(keyEncrypt, 0, 8);

      if (key.length() <= 8)
        memcpy(keyEncrypt, key.c_str(), key.length());
      else
        memcpy(keyEncrypt, key.c_str(), 8);

      DES_key_schedule keySchedule;
      DES_set_key_unchecked(&keyEncrypt, &keySchedule);

      const_DES_cblock inputText;
      DES_cblock outputText;
      std::vector<unsigned char> vecCleartext;
      unsigned char tmp[8];

      for (int i = 0; i < ciphertext.length() / 8; i++) {
        memcpy(inputText, ciphertext.c_str() + i * 8, 8);
        DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
        memcpy(tmp, outputText, 8);

        for (int j = 0; j < 8; j++) vecCleartext.push_back(tmp[j]);
      }

      if (ciphertext.length() % 8 != 0) {
        int tmp1 = ciphertext.length() / 8 * 8;
        int tmp2 = ciphertext.length() - tmp1;
        memset(inputText, 0, 8);
        memcpy(inputText, ciphertext.c_str() + tmp1, tmp2);

        DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
        memcpy(tmp, outputText, 8);

        for (int j = 0; j < 8; j++) vecCleartext.push_back(tmp[j]);
      }

      strClearText.clear();
      strClearText.assign(vecCleartext.begin(), vecCleartext.end());
    } break;
  }

  return strClearText;
}

char *rsa_encrypt(const unsigned char *str, const char *public_key_filename,
                  int &len) {
  char *p_en = NULL;
  RSA *p_rsa = NULL;
  FILE *pf = NULL;
  int rsa_len = 0;

  do {
    if ((pf = fopen(public_key_filename, "rb")) == NULL) break;

    if ((p_rsa = PEM_read_RSA_PUBKEY(pf, NULL, NULL, NULL)) == NULL) break;

    rsa_len = RSA_size(p_rsa);
    p_en = static_cast<char *>(malloc(rsa_len + 1));
    memset(p_en, 0, rsa_len + 1);
    if ((len = RSA_public_encrypt(rsa_len, str, (unsigned char *)p_en, p_rsa,
                                  RSA_NO_PADDING)) < 0)
      break;

  } while (0);

  RSA_free(p_rsa);
  if (pf) fclose(pf);

  return p_en;
}

char *rsa_decrypt(const unsigned char *str, const char *private_key_filename,
                  int &len) {
  char *p_de = NULL;
  RSA *p_rsa = NULL;
  FILE *pf = NULL;
  int rsa_len = 0;

  do {
    if ((pf = fopen(private_key_filename, "rb")) == NULL) break;

    if ((p_rsa = PEM_read_RSAPrivateKey(pf, NULL, NULL, NULL)) == NULL) break;

    rsa_len = RSA_size(p_rsa);
    p_de = static_cast<char *>(malloc(rsa_len + 1));
    memset(p_de, 0, rsa_len + 1);
    if ((len = RSA_private_decrypt(rsa_len, str, (unsigned char *)p_de, p_rsa,
                                   RSA_NO_PADDING)) < 0)
      break;
  } while (0);

  RSA_free(p_rsa);
  if (pf) fclose(pf);

  return p_de;
}

int main(int argc, char *argv[]) { return 0; }

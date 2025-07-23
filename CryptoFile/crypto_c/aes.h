#ifndef AES_H
#define AES_H

#ifdef __cplusplus
extern "C" {
#endif

// AES暗号化関数
int aes_encrypt(const unsigned char* input, int length, 
                const unsigned char* key, unsigned char* output);

// AES復号化関数
int aes_decrypt(const unsigned char* input, int length, 
                const unsigned char* key, unsigned char* output);

// キーハッシュ生成（パスワードから256ビットキーを生成）
void generate_key_from_password(const char* password, unsigned char* key);

#ifdef __cplusplus
}
#endif

#endif // AES_H

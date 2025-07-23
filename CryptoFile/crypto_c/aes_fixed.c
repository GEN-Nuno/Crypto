#include "aes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 簡単なXOR暗号化（デモ用）
// 実際のプロダクションではAESライブラリを使用してください
static void simple_xor_crypt(const unsigned char* input, int length,
                             const unsigned char* key, unsigned char* output) {
    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % 32]; // 32バイト（256ビット）キーを循環使用
    }
}

// SHA-256の簡易実装（デモ用）
// 実際のプロダクションでは適切なハッシュライブラリを使用してください
static void simple_hash(const char* input, unsigned char* output) {
    // 簡単なハッシュ（XORベース）
    memset(output, 0, 32);
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i % 32] ^= (unsigned char)input[i];
        output[(i + 1) % 32] ^= (unsigned char)(input[i] << 1);
        output[(i + 2) % 32] ^= (unsigned char)(input[i] >> 1);
    }
}

// パスワードから256ビットキーを生成
void generate_key_from_password(const char* password, unsigned char* key) {
    simple_hash(password, key);
}

// 暗号化関数（XORベース、デモ用）
int aes_encrypt(const unsigned char* input, int length,
                const unsigned char* key, unsigned char* output) {
    if (!input || !key || !output || length <= 0) {
        return -1; // エラー
    }
    
    // 簡単なXOR暗号化
    simple_xor_crypt(input, length, key, output);
    
    return length; // 暗号化後のサイズ（同じサイズ）
}

// 復号化関数（XORベース、デモ用）
int aes_decrypt(const unsigned char* input, int length,
                const unsigned char* key, unsigned char* output) {
    if (!input || !key || !output || length <= 0) {
        return -1; // エラー
    }
    
    // XOR暗号化は復号化と同じ操作
    simple_xor_crypt(input, length, key, output);
    
    return length; // 復号化後のサイズ（同じサイズ）
}

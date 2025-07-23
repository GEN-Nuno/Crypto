#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <string>
#include <vector>

class CryptoManager {
public:
    CryptoManager();
    ~CryptoManager();
    
    // ファイル暗号化
    bool encryptFile(const std::string& inputPath, 
                    const std::string& outputPath, 
                    const std::string& password);
    
    // ファイル復号化
    bool decryptFile(const std::string& inputPath, 
                    const std::string& outputPath, 
                    const std::string& password);
    
    // 複数ファイル暗号化
    bool encryptFiles(const std::vector<std::string>& inputPaths,
                     const std::string& outputDir,
                     const std::string& password);
    
    // 複数ファイル復号化
    bool decryptFiles(const std::vector<std::string>& inputPaths,
                     const std::string& outputDir,
                     const std::string& password);
    
    // エラーメッセージ取得
    std::string getLastError() const;

private:
    std::string lastError;
    
    // ファイル読み込み
    std::vector<unsigned char> readFile(const std::string& path);
    
    // ファイル書き込み
    bool writeFile(const std::string& path, const std::vector<unsigned char>& data);
};

// C#からの呼び出し用のC API
extern "C" {
    __declspec(dllexport) int crypto_encrypt_file(const char* input_path, 
                                                  const char* output_path, 
                                                  const char* password);
    
    __declspec(dllexport) int crypto_decrypt_file(const char* input_path, 
                                                  const char* output_path, 
                                                  const char* password);
    
    __declspec(dllexport) const char* crypto_get_last_error();
}

#endif // CRYPTOMANAGER_H

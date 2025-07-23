#include "CryptoManager.h"
#include "../crypto_c/aes.h"
#include <fstream>
#include <iostream>
#include <filesystem>

CryptoManager::CryptoManager() {
}

CryptoManager::~CryptoManager() {
}

std::vector<unsigned char> CryptoManager::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        lastError = "Failed to open file: " + path;
        return {};
    }
    
    // ファイルサイズを取得
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // データを読み込み
    std::vector<unsigned char> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    if (!file.good()) {
        lastError = "Failed to read file: " + path;
        return {};
    }
    
    return data;
}

bool CryptoManager::writeFile(const std::string& path, const std::vector<unsigned char>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        lastError = "Failed to create file: " + path;
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    
    if (!file.good()) {
        lastError = "Failed to write file: " + path;
        return false;
    }
    
    return true;
}

bool CryptoManager::encryptFile(const std::string& inputPath, 
                               const std::string& outputPath, 
                               const std::string& password) {
    // ファイルを読み込み
    auto inputData = readFile(inputPath);
    if (inputData.empty()) {
        return false;
    }
    
    // パスワードからキーを生成
    unsigned char key[32]; // 256ビット
    generate_key_from_password(password.c_str(), key);
    
    // 暗号化後のデータサイズを計算（16バイト境界に合わせる）
    int encryptedSize = ((inputData.size() + 15) / 16) * 16;
    std::vector<unsigned char> encryptedData(encryptedSize);
    
    // 暗号化実行
    int result = aes_encrypt(inputData.data(), inputData.size(), 
                            key, encryptedData.data());
    
    if (result < 0) {
        lastError = "Encryption failed";
        return false;
    }
    
    // 暗号化されたデータをファイルに書き込み
    encryptedData.resize(result);
    return writeFile(outputPath, encryptedData);
}

bool CryptoManager::decryptFile(const std::string& inputPath, 
                               const std::string& outputPath, 
                               const std::string& password) {
    // ファイルを読み込み
    auto inputData = readFile(inputPath);
    if (inputData.empty()) {
        return false;
    }
    
    // パスワードからキーを生成
    unsigned char key[32]; // 256ビット
    generate_key_from_password(password.c_str(), key);
    
    // 復号化後のデータバッファを準備
    std::vector<unsigned char> decryptedData(inputData.size());
    
    // 復号化実行
    int result = aes_decrypt(inputData.data(), inputData.size(), 
                            key, decryptedData.data());
    
    if (result < 0) {
        lastError = "Decryption failed";
        return false;
    }
    
    // 復号化されたデータをファイルに書き込み
    decryptedData.resize(result);
    return writeFile(outputPath, decryptedData);
}

bool CryptoManager::encryptFiles(const std::vector<std::string>& inputPaths,
                                const std::string& outputDir,
                                const std::string& password) {
    for (const auto& inputPath : inputPaths) {
        std::filesystem::path input(inputPath);
        std::filesystem::path output(outputDir);
        output /= input.filename().string() + ".enc";
        
        if (!encryptFile(inputPath, output.string(), password)) {
            return false;
        }
    }
    return true;
}

bool CryptoManager::decryptFiles(const std::vector<std::string>& inputPaths,
                                const std::string& outputDir,
                                const std::string& password) {
    for (const auto& inputPath : inputPaths) {
        std::filesystem::path input(inputPath);
        std::filesystem::path output(outputDir);
        
        std::string filename = input.filename().string();
        if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".enc") {
            filename = filename.substr(0, filename.size() - 4);
        }
        output /= filename;
        
        if (!decryptFile(inputPath, output.string(), password)) {
            return false;
        }
    }
    return true;
}

std::string CryptoManager::getLastError() const {
    return lastError;
}

// C API実装（C#からの呼び出し用）
static CryptoManager* g_manager = nullptr;

extern "C" {
    __declspec(dllexport) int crypto_encrypt_file(const char* input_path, 
                                                  const char* output_path, 
                                                  const char* password) {
        if (!g_manager) {
            g_manager = new CryptoManager();
        }
        
        return g_manager->encryptFile(input_path, output_path, password) ? 1 : 0;
    }
    
    __declspec(dllexport) int crypto_decrypt_file(const char* input_path, 
                                                  const char* output_path, 
                                                  const char* password) {
        if (!g_manager) {
            g_manager = new CryptoManager();
        }
        
        return g_manager->decryptFile(input_path, output_path, password) ? 1 : 0;
    }
    
    __declspec(dllexport) const char* crypto_get_last_error() {
        if (!g_manager) {
            return "Manager not initialized";
        }
        
        static std::string error;
        error = g_manager->getLastError();
        return error.c_str();
    }
}

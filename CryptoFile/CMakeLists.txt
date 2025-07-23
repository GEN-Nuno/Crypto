cmake_minimum_required(VERSION 3.16)
project(CryptoFile)

# C++17を有効にする
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenSSLを探す
find_package(OpenSSL REQUIRED)

# Cソースファイル
set(C_SOURCES
    crypto_c/aes.c
)

# C++ソースファイル
set(CPP_SOURCES
    crypto_cpp/CryptoManager.cpp
)

# 共有ライブラリ（DLL）として作成
add_library(CryptoFile SHARED ${C_SOURCES} ${CPP_SOURCES})

# OpenSSLをリンク
target_link_libraries(CryptoFile OpenSSL::SSL OpenSSL::Crypto)

# ヘッダーファイルのインクルードパス
target_include_directories(CryptoFile PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/crypto_c
    ${CMAKE_CURRENT_SOURCE_DIR}/crypto_cpp
)

# Windowsの場合の設定
if(WIN32)
    # DLLエクスポート用のマクロを定義
    target_compile_definitions(CryptoFile PRIVATE BUILDING_DLL)
    
    # CRTライブラリの設定
    set_property(TARGET CryptoFile PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# デバッグ情報を含める
set(CMAKE_BUILD_TYPE Debug)

# 出力ディレクトリを設定
set_target_properties(CryptoFile PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

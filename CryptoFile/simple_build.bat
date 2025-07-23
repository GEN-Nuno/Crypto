@echo off
echo CryptoFile シンプルビルドスクリプト
echo ===============================

:: ビルドディレクトリの作成
if not exist build mkdir build

:: C++でDLLをビルド（g++を使用）
echo C++/CでDLLをビルド中...
g++ -shared -o build\CryptoFile.dll crypto_c\aes.c crypto_cpp\CryptoManager.cpp -std=c++17 -DBUILDING_DLL

if %ERRORLEVEL% neq 0 (
    echo DLLのビルドに失敗しました。
    echo MinGW-w64がインストールされていることを確認してください。
    echo 以下のコマンドでインストールできます:
    echo winget install -e --id MSYS2.MSYS2
    echo その後、MSYS2でg++をインストールしてください。
    pause
    exit /b 1
)

echo DLLのビルドが完了しました: build\CryptoFile.dll

:: C#プロジェクトのビルド
cd gui_csharp
echo C# GUIアプリケーションをビルド中...
dotnet build --configuration Release

if %ERRORLEVEL% neq 0 (
    echo C#プロジェクトのビルドに失敗しました。
    echo .NET 6.0 SDKがインストールされていることを確認してください。
    pause
    exit /b 1
)

:: DLLを実行ファイルのディレクトリにコピー
copy ..\build\CryptoFile.dll bin\Release\net6.0-windows\

echo.
echo 全てのビルドが完了しました！
echo 実行ファイル: %cd%\bin\Release\net6.0-windows\CryptoFileGUI.exe
echo.
pause

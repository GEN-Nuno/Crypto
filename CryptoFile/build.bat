@echo off
echo CryptoFile ビルドスクリプト
echo ========================

:: ビルドディレクトリの作成
if not exist build mkdir build
cd build

:: CMakeでプロジェクトを生成
echo CMakeでプロジェクトを生成中...
cmake .. -G "Visual Studio 17 2022" -A x64

if %ERRORLEVEL% neq 0 (
    echo CMakeの生成に失敗しました。
    echo Visual Studio 2022およびOpenSSLがインストールされていることを確認してください。
    pause
    exit /b 1
)

:: ビルド実行
echo プロジェクトをビルド中...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo ビルドに失敗しました。
    pause
    exit /b 1
)

echo.
echo ビルドが完了しました！
echo DLL: %cd%\Release\CryptoFile.dll
echo.

:: C#プロジェクトのビルド
cd ..\gui_csharp
echo C# GUIアプリケーションをビルド中...
dotnet build --configuration Release

if %ERRORLEVEL% neq 0 (
    echo C#プロジェクトのビルドに失敗しました。
    pause
    exit /b 1
)

:: DLLを実行ファイルのディレクトリにコピー
copy ..\build\Release\CryptoFile.dll bin\Release\net6.0-windows\

echo.
echo 全てのビルドが完了しました！
echo 実行ファイル: %cd%\bin\Release\net6.0-windows\CryptoFileGUI.exe
echo.
pause

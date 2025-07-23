@echo off
echo CryptoFile 実行スクリプト
echo ======================

:: DLLが存在するかチェック
if not exist "gui_csharp\bin\Release\net6.0-windows\CryptoFile.dll" (
    echo CryptoFile.dll が見つかりません。
    echo 先にbuild.batを実行してプロジェクトをビルドしてください。
    pause
    exit /b 1
)

::実行ファイルが存在するかチェック
if not exist "gui_csharp\bin\Release\net6.0-windows\CryptoFileGUI.exe" (
    echo CryptoFileGUI.exe が見つかりません。
    echo 先にbuild.batを実行してプロジェクトをビルドしてください。
    pause
    exit /b 1
)

:: アプリケーションを起動
echo CryptoFileを起動中...
start "" "gui_csharp\bin\Release\net6.0-windows\CryptoFileGUI.exe"

echo アプリケーションが起動しました。

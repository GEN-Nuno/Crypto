@echo off
echo CryptoFile 実行スクリプト
echo ======================

:: 実行ファイルの存在確認
if not exist "gui_csharp\bin\Release\net6.0-windows\CryptoFileGUI.exe" (
    echo 実行ファイルが見つかりません。
    echo 先にsimple_build.batを実行してビルドしてください。
    pause
    exit /b 1
)

:: DLLの存在確認
if not exist "gui_csharp\bin\Release\net6.0-windows\CryptoFile.dll" (
    echo CryptoFile.dll が見つかりません。
    echo 先にsimple_build.batを実行してビルドしてください。
    pause
    exit /b 1
)

:: アプリケーションを起動
echo CryptoFileを起動中...
start "" "gui_csharp\bin\Release\net6.0-windows\CryptoFileGUI.exe"

echo アプリケーションが起動しました。
echo.
echo 使用方法:
echo 1. ファイル選択ボタンで暗号化/復号化したいファイルを選択
echo 2. 出力フォルダを指定
echo 3. パスワードを入力
echo 4. 暗号化または復号化ボタンをクリック
echo.
echo 注意: これはデモ版です。XOR暗号化を使用しており、
echo      実際のセキュリティ用途には適していません。

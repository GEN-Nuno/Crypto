---

## 🔐 プロジェクト概要：ファイル暗号化ソフト（CryptoFile）

### 📁 機能

* ファイルを読み取り、暗号化して保存（暗号化形式は AES）
* 復号も可能（鍵を入力）
* GUIでファイル選択・鍵入力・進捗表示
* 複数ファイルを一括処理可能

---

## 🧩 技術構成（3言語連携）

| 言語      | 役割                 | 内容                              |
| ------- | ------------------ | ------------------------------- |
| **C**   | 暗号処理               | OpenSSLやmbedTLSなどでAES暗号を実装      |
| **C++** | 暗号ライブラリのラップ・ファイル処理 | ファイルの読み込み、バッファ管理、CのAES関数を呼び出し   |
| **C#**  | GUIアプリケーション        | ユーザー操作（ファイル選択・鍵入力）、DLL呼び出し、進捗表示 |

---

## 🔗 連携の流れ

1. C で `aes_encrypt` / `aes_decrypt` 関数（バッファ入力・バッファ出力）
2. C++ 側で `CryptoManager` クラスがファイルのバイナリを読み、C関数を呼び出す
3. C++ を DLL としてビルド → C# から `DllImport` で利用 or C++/CLI ラッパークラス経由
4. C# でファイル選択ダイアログ、パスワード入力UI、実行ボタンを用意

---

## 🔧 フォルダ構成案

```
CryptoFile/
├─ crypto_c/              # C: AES暗号化処理（libcrypto）
│   └─ aes.c / aes.h
├─ crypto_cpp/            # C++: ファイル処理 + AES呼び出し
│   └─ CryptoManager.cpp / .h
│   └─ CryptoFile.dll     # C++でDLL化（C#用）
├─ gui_csharp/            # C#: GUIアプリ（WPF）
│   └─ MainWindow.xaml / .cs
├─ build/                 # 出力ディレクトリ
└─ README.md
```

---

## ✨ UIイメージ（C# WPF）

* ファイルパス（複数選択）
* パスワード入力欄（鍵として使用）
* 「暗号化」ボタン／「復号」ボタン
* 進捗バー＋ログ表示欄

---

## 🔑 暗号方式（C）

```c
// aes.h
void aes_encrypt(const unsigned char* input, int length,
                 const unsigned char* key, unsigned char* output);

void aes_decrypt(const unsigned char* input, int length,
                 const unsigned char* key, unsigned char* output);
```

---

## ✅ 実装の流れ

1. CでAES暗号関数（OpenSSLまたは独自実装）
2. C++でファイル読み書き＋C関数呼び出し
3. C++ DLL作成（`extern "C"` で C# に公開）
4. C# WPFでGUI作成＋DLL呼び出し

---

## その他

* [ ] CのAES暗号関数の雛形を作成
* [ ] C++で `CryptoManager` 実装
* [ ] DLLインターフェース定義（C++ → C#）
* [ ] C# GUIサンプルの用意

---
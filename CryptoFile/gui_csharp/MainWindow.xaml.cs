using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;

namespace CryptoFileGUI
{
    public partial class MainWindow : Window
    {
        private List<string> selectedFiles = new List<string>();

        // DLL Import
        [DllImport("CryptoFile.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int crypto_encrypt_file(string input_path, string output_path, string password);

        [DllImport("CryptoFile.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int crypto_decrypt_file(string input_path, string output_path, string password);

        [DllImport("CryptoFile.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr crypto_get_last_error();

        public MainWindow()
        {
            InitializeComponent();
            InitializeOutputFolder();
            AddLog("CryptoFileが起動しました。");
        }

        private void InitializeOutputFolder()
        {
            txtOutputFolder.Text = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "CryptoFile_Output");
        }

        private void BtnSelectFiles_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Title = "暗号化/復号化するファイルを選択",
                Filter = "すべてのファイル (*.*)|*.*",
                Multiselect = true
            };

            if (openFileDialog.ShowDialog() == true)
            {
                foreach (string filename in openFileDialog.FileNames)
                {
                    if (!selectedFiles.Contains(filename))
                    {
                        selectedFiles.Add(filename);
                    }
                }
                UpdateFileList();
                AddLog($"{openFileDialog.FileNames.Length}個のファイルが選択されました。");
            }
        }

        private void BtnClearFiles_Click(object sender, RoutedEventArgs e)
        {
            selectedFiles.Clear();
            UpdateFileList();
            AddLog("ファイルリストがクリアされました。");
        }

        private void BtnSelectOutputFolder_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.Title = "出力フォルダを選択してください";
            dialog.FileName = "フォルダを選択";
            dialog.DefaultExt = "";
            dialog.Filter = "";
            
            var result = dialog.ShowDialog();
            if (result == true)
            {
                string folderPath = System.IO.Path.GetDirectoryName(dialog.FileName);
                txtOutputFolder.Text = folderPath;
                AddLog($"出力フォルダが設定されました: {folderPath}");
            }
        }

        private void UpdateFileList()
        {
            listFiles.ItemsSource = null;
            listFiles.ItemsSource = selectedFiles.Select(f => Path.GetFileName(f)).ToList();
        }

        private void ChkShowPassword_Checked(object sender, RoutedEventArgs e)
        {
            txtPasswordVisible.Text = pwdPassword.Password;
            txtPasswordVisible.Visibility = Visibility.Visible;
            pwdPassword.Visibility = Visibility.Collapsed;
        }

        private void ChkShowPassword_Unchecked(object sender, RoutedEventArgs e)
        {
            pwdPassword.Password = txtPasswordVisible.Text;
            pwdPassword.Visibility = Visibility.Visible;
            txtPasswordVisible.Visibility = Visibility.Collapsed;
        }

        private async void BtnEncrypt_Click(object sender, RoutedEventArgs e)
        {
            await ProcessFiles(true);
        }

        private async void BtnDecrypt_Click(object sender, RoutedEventArgs e)
        {
            await ProcessFiles(false);
        }

        private async Task ProcessFiles(bool encrypt)
        {
            if (selectedFiles.Count == 0)
            {
                MessageBox.Show("ファイルが選択されていません。", "エラー", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            string password = chkShowPassword.IsChecked == true ? txtPasswordVisible.Text : pwdPassword.Password;
            if (string.IsNullOrEmpty(password))
            {
                MessageBox.Show("パスワードを入力してください。", "エラー", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            string outputDir = txtOutputFolder.Text;
            if (string.IsNullOrEmpty(outputDir))
            {
                MessageBox.Show("出力フォルダを指定してください。", "エラー", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                Directory.CreateDirectory(outputDir);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"出力フォルダの作成に失敗しました: {ex.Message}", "エラー", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            SetUIEnabled(false);
            progressBar.Value = 0;
            progressBar.Maximum = selectedFiles.Count;

            string operation = encrypt ? "暗号化" : "復号化";
            AddLog($"{operation}を開始します... ({selectedFiles.Count}ファイル)");

            try
            {
                await Task.Run(() =>
                {
                    for (int i = 0; i < selectedFiles.Count; i++)
                    {
                        string inputFile = selectedFiles[i];
                        string fileName = Path.GetFileName(inputFile);
                        string outputFile;

                        if (encrypt)
                        {
                            outputFile = Path.Combine(outputDir, fileName + ".enc");
                        }
                        else
                        {
                            if (fileName.EndsWith(".enc"))
                            {
                                fileName = fileName.Substring(0, fileName.Length - 4);
                            }
                            outputFile = Path.Combine(outputDir, fileName);
                        }

                        Dispatcher.Invoke(() =>
                        {
                            lblStatus.Text = $"{operation}中... {Path.GetFileName(inputFile)}";
                            AddLog($"{operation}中: {Path.GetFileName(inputFile)}");
                        });

                        int result;
                        if (encrypt)
                        {
                            result = crypto_encrypt_file(inputFile, outputFile, password);
                        }
                        else
                        {
                            result = crypto_decrypt_file(inputFile, outputFile, password);
                        }

                        if (result == 1)
                        {
                            Dispatcher.Invoke(() => AddLog($"✓ 完了: {Path.GetFileName(outputFile)}"));
                        }
                        else
                        {
                            IntPtr errorPtr = crypto_get_last_error();
                            string error = Marshal.PtrToStringAnsi(errorPtr) ?? "不明なエラー";
                            Dispatcher.Invoke(() => AddLog($"✗ エラー: {Path.GetFileName(inputFile)} - {error}"));
                        }

                        Dispatcher.Invoke(() => progressBar.Value = i + 1);
                    }
                });

                AddLog($"{operation}が完了しました。");
                lblStatus.Text = $"{operation}完了";
                MessageBox.Show($"{operation}が完了しました。\n出力先: {outputDir}", "完了", 
                               MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                AddLog($"エラーが発生しました: {ex.Message}");
                MessageBox.Show($"エラーが発生しました: {ex.Message}", "エラー", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
            finally
            {
                SetUIEnabled(true);
                progressBar.Value = 0;
                lblStatus.Text = "準備完了";
            }
        }

        private void SetUIEnabled(bool enabled)
        {
            btnSelectFiles.IsEnabled = enabled;
            btnClearFiles.IsEnabled = enabled;
            btnSelectOutputFolder.IsEnabled = enabled;
            btnEncrypt.IsEnabled = enabled;
            btnDecrypt.IsEnabled = enabled;
            pwdPassword.IsEnabled = enabled;
            txtPasswordVisible.IsEnabled = enabled;
            chkShowPassword.IsEnabled = enabled;
        }

        private void AddLog(string message)
        {
            string timestamp = DateTime.Now.ToString("HH:mm:ss");
            txtLog.Text += $"[{timestamp}] {message}\n";
            txtLog.ScrollToEnd();
        }
    }
}

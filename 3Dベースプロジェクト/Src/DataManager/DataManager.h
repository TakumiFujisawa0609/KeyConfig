#pragma once

#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <mutex>

// シーンごとのデータを管理するシングルトン
class DataManager
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new DataManager(); } };
    static DataManager* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }
private:
    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    DataManager(void);
    // デストラクタも同様
    ~DataManager(void);

    // コピー・ムーブ操作を禁止
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager(DataManager&&) = delete;
    DataManager& operator=(DataManager&&) = delete;

    // 下記をコンパイルエラーさせるため 上記を追加
    // DataManager copy = *DataManager::GetInstance();
    // DataManager copied(*DataManager::GetInstance());
    // DataManager moved = std::move(*DataManager::GetInstance());

public:
    /// <summary>
    /// 指定したシーン名とフォルダパスをロードする
    /// </summary>
    /// <param name="sceneName">シーン名（例: "TitleScene"）</param>
    /// <param name="folderPath">ロードするフォルダパス</param>
    void LoadSceneData(const std::string& sceneName, const std::string& folderPath);

    /// <summary>
    /// 指定フォルダ内のすべてのサブフォルダをシーンとしてロードする
    /// </summary>
    /// <param name="rootPath">rootPath 基準フォルダ（例: "Data"）</param>
    /// <param name="overwrite">overwrite 既に存在するシーン名があった場合に上書きするか（デフォルトfalseでスキップ）</param>
    void LoadAllData(const std::string& rootPath, bool overwrite = false);

    /// <summary>
    /// シーン名とフォルダ名でファイル一覧を取得する
    /// </summary>
    /// <param name="sceneName">シーン名</param>
    /// <param name="folderName">フォルダ名（相対パス）</param>
    /// <returns>ファイルパスのリスト</returns>
    const std::vector<std::string>& GetFiles(const std::string& sceneName, const std::string& folderName) const;

    /// <summary>
    /// 複数拡張子フィルタ付きでファイル一覧を取得する
    /// </summary>
    std::vector<std::string> GetFilesWithExtensions(
        const std::string& sceneName,
        const std::string& folderName,
        const std::vector<std::string>& extensions) const;

    /// <summary>
    /// ロード済みデータをすべて表示
    /// </summary>
    void PrintAll() const;

    /// <summary>
    /// 指定したシーンのデータを削除する
    /// </summary>
    /// <param name="sceneName">削除するシーン名</param>
    void RemoveSceneData(const std::string& sceneName);

    /// <summary>
    /// すべてのシーンデータを削除する
    /// </summary>
    void ClearAll();

private:
    // 静的インスタンス
    static DataManager* instance_;

    // データ構造: シーン名 -> (フォルダ名 -> ファイルパスリスト)
    std::map<std::string, std::map<std::string, std::vector<std::string>>> sceneData_;

    // スレッド安全用
    mutable std::mutex mutex_;
};
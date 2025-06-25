#include "DataManager.h"

#include <DxLib.h>
#include <iostream>

#include "../Common/Debug.h"

DataManager* DataManager::instance_ = nullptr;

namespace fs = std::filesystem;

DataManager::DataManager(void) {};
DataManager::~DataManager(void) {};

/// <summary>
/// 指定したシーン名とフォルダパスを再帰的に探索してファイルをグループ化してロードする
/// </summary>
/// <param name="sceneName">シーン名（例: "TitleScene"）</param>
/// <param name="folderPath">ロードする基準フォルダパス</param>
void DataManager::LoadSceneData(const std::string& sceneName, const std::string& folderPath)
{
    std::error_code ec;
    std::map<std::string, std::vector<std::string>> groupedFiles;

    // フォルダが存在するか確認
    if (fs::exists(folderPath, ec))
    {
        // 再帰的にディレクトリを探索
        for (fs::recursive_directory_iterator it(folderPath, ec), end; it != end; it.increment(ec))
        {
            if (ec) continue; // 途中でエラーがあればスキップ

            if (it->is_regular_file(ec)) // 通常のファイルのみ対象
            {
                fs::path absPath = it->path();
                // 基準フォルダからの相対パスを取得
                fs::path relative = fs::relative(absPath, folderPath, ec);
                if (ec) continue;

                // 相対パスの親ディレクトリをキーとする
                std::string folderName = relative.parent_path().string();
                if (folderName.empty())
                {
                    folderName = "."; // ルート直下の場合
                }

                // グループに追加
                groupedFiles[folderName].push_back(absPath.string());
            }
        }

        // 探索結果をシーン名で登録（排他ロック）
        std::lock_guard<std::mutex> lock(mutex_);
        sceneData_[sceneName] = groupedFiles;
    }
    else
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "[ERROR] Folder not found: %s", folderPath.c_str());
        LogMessage(buf);
    }
}

/// <summary>
/// 指定フォルダ内のすべてのサブフォルダをシーンとしてロードする
/// </summary>
/// <param name="rootPath">rootPath 基準フォルダ（例: "Data"）</param>
/// <param name="overwrite">overwrite 既に存在するシーン名があった場合に上書きするか（デフォルトfalseでスキップ）</param>
void DataManager::LoadAllData(const std::string& rootPath, bool overwrite)
{
    std::error_code ec;

    if (!fs::exists(rootPath, ec))
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "[ERROR] Root folder not found: %s", rootPath.c_str());
        LogMessage(buf);

        return;
    }

    for (fs::directory_iterator it(rootPath, ec), end; it != end; it.increment(ec))
    {
        if (ec) continue;

        if (it->is_directory(ec))
        {
            std::string sceneName = it->path().filename().string();

            {
                std::lock_guard<std::mutex> lock(mutex_);

                // シーンが既に存在しているかチェック
                bool exists = (sceneData_.find(sceneName) != sceneData_.end());
                if (exists && !overwrite)
                {
                    // スキップするのでログを出す
                    char buf[512];
                    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' already exists. Skipping.", sceneName.c_str());
                    LogMessage(buf);

                    continue;
                }
                else if (exists && overwrite)
                {
                    // 上書きのため既存データを削除
                    sceneData_.erase(sceneName);

                    char buf[512];
                    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' will be overwritten.", sceneName.c_str());
                    LogMessage(buf);
                }
            }

            // シーンロードはロック外で実施（loadSceneData内で再度ロック）
            LoadSceneData(sceneName, it->path().string());
        }
    }
}

/// <summary>
/// シーン名とフォルダ名でファイル一覧を取得する
/// データが存在しない場合は空のリストを返す
/// </summary>
/// <param name="sceneName"></param>
/// <param name="folderName">フォルダ名（相対パス）フォルダ名（相対パス）</param>
/// <returns>ファイルパスのリスト</returns>
const std::vector<std::string>& DataManager::GetFiles(const std::string& sceneName, const std::string& folderName) const
{
    static const std::vector<std::string> empty;

    auto sceneIt = sceneData_.find(sceneName);
    if (sceneIt != sceneData_.end())
    {
        auto folderIt = sceneIt->second.find(folderName);
        if (folderIt != sceneIt->second.end())
        {
            return folderIt->second;
        }
    }
    return empty; // 見つからない場合は空のリスト
}

/// <summary>
/// 複数拡張子フィルタ付きでファイル一覧を取得する
/// </summary>
/// <param name="sceneName">シーン名</param>
/// <param name="folderName">フォルダ名</param>
/// <param name="extensions">拡張子リスト（例: { ".png", ".jpg" }）</param>
/// <returns>フィルタされたファイルリスト</returns>
std::vector<std::string> DataManager::GetFilesWithExtensions(const std::string& sceneName, const std::string& folderName, const std::vector<std::string>& extensions) const
{
    std::vector<std::string> filtered;

    const auto& files = GetFiles(sceneName, folderName);

    for (const auto& file : files)
    {
        fs::path p(file);

        for (const auto& ext : extensions)
        {
            if (p.extension() == ext)
            {
                filtered.push_back(file);
                break; // どれか一致したら追加して次へ
            }
        }
    }

    return filtered;
}


/// <summary>
/// ロード済みのすべてのシーンデータを標準出力に表示する
/// </summary>
void DataManager::PrintAll() const
{
    int y = 0;

    for (const auto& [scene, folders] : sceneData_)
    {
        DrawFormatString(0, y, GetColor(255, 255, 255), "Scene: %s", scene.c_str());
        y += 20;

        for (const auto& [folder, files] : folders)
        {
            DrawFormatString(20, y, GetColor(255, 255, 255), "Folder: %s", folder.c_str());
            y += 20;

            for (const auto& f : files)
            {
                DrawFormatString(40, y, GetColor(255, 255, 255), "File: %s", f.c_str());
                y += 20;
            }
        }
    }
}

/// <summary>
/// 指定したシーンのデータを削除する
/// </summary>
/// <param name="sceneName">削除するシーン名</param>
void DataManager::RemoveSceneData(const std::string& sceneName)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sceneData_.erase(sceneName);

    char buf[256];
    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' removed.", sceneName.c_str());
    LogMessage(buf);
}

/// <summary>
/// すべてのシーンデータを削除する
/// </summary>
void DataManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(mutex_);
    sceneData_.clear();

    LogMessage("[INFO] All scenes cleared.");
}

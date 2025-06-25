#include "DataManager.h"

#include <DxLib.h>
#include <iostream>

#include "../Common/Debug.h"

DataManager* DataManager::instance_ = nullptr;

namespace fs = std::filesystem;

DataManager::DataManager(void) {};
DataManager::~DataManager(void) {};

/// <summary>
/// �w�肵���V�[�����ƃt�H���_�p�X���ċA�I�ɒT�����ăt�@�C�����O���[�v�����ă��[�h����
/// </summary>
/// <param name="sceneName">�V�[�����i��: "TitleScene"�j</param>
/// <param name="folderPath">���[�h�����t�H���_�p�X</param>
void DataManager::LoadSceneData(const std::string& sceneName, const std::string& folderPath)
{
    std::error_code ec;
    std::map<std::string, std::vector<std::string>> groupedFiles;

    // �t�H���_�����݂��邩�m�F
    if (fs::exists(folderPath, ec))
    {
        // �ċA�I�Ƀf�B���N�g����T��
        for (fs::recursive_directory_iterator it(folderPath, ec), end; it != end; it.increment(ec))
        {
            if (ec) continue; // �r���ŃG���[������΃X�L�b�v

            if (it->is_regular_file(ec)) // �ʏ�̃t�@�C���̂ݑΏ�
            {
                fs::path absPath = it->path();
                // ��t�H���_����̑��΃p�X���擾
                fs::path relative = fs::relative(absPath, folderPath, ec);
                if (ec) continue;

                // ���΃p�X�̐e�f�B���N�g�����L�[�Ƃ���
                std::string folderName = relative.parent_path().string();
                if (folderName.empty())
                {
                    folderName = "."; // ���[�g�����̏ꍇ
                }

                // �O���[�v�ɒǉ�
                groupedFiles[folderName].push_back(absPath.string());
            }
        }

        // �T�����ʂ��V�[�����œo�^�i�r�����b�N�j
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
/// �w��t�H���_���̂��ׂẴT�u�t�H���_���V�[���Ƃ��ă��[�h����
/// </summary>
/// <param name="rootPath">rootPath ��t�H���_�i��: "Data"�j</param>
/// <param name="overwrite">overwrite ���ɑ��݂���V�[�������������ꍇ�ɏ㏑�����邩�i�f�t�H���gfalse�ŃX�L�b�v�j</param>
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

                // �V�[�������ɑ��݂��Ă��邩�`�F�b�N
                bool exists = (sceneData_.find(sceneName) != sceneData_.end());
                if (exists && !overwrite)
                {
                    // �X�L�b�v����̂Ń��O���o��
                    char buf[512];
                    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' already exists. Skipping.", sceneName.c_str());
                    LogMessage(buf);

                    continue;
                }
                else if (exists && overwrite)
                {
                    // �㏑���̂��ߊ����f�[�^���폜
                    sceneData_.erase(sceneName);

                    char buf[512];
                    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' will be overwritten.", sceneName.c_str());
                    LogMessage(buf);
                }
            }

            // �V�[�����[�h�̓��b�N�O�Ŏ��{�iloadSceneData���ōēx���b�N�j
            LoadSceneData(sceneName, it->path().string());
        }
    }
}

/// <summary>
/// �V�[�����ƃt�H���_���Ńt�@�C���ꗗ���擾����
/// �f�[�^�����݂��Ȃ��ꍇ�͋�̃��X�g��Ԃ�
/// </summary>
/// <param name="sceneName"></param>
/// <param name="folderName">�t�H���_���i���΃p�X�j�t�H���_���i���΃p�X�j</param>
/// <returns>�t�@�C���p�X�̃��X�g</returns>
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
    return empty; // ������Ȃ��ꍇ�͋�̃��X�g
}

/// <summary>
/// �����g���q�t�B���^�t���Ńt�@�C���ꗗ���擾����
/// </summary>
/// <param name="sceneName">�V�[����</param>
/// <param name="folderName">�t�H���_��</param>
/// <param name="extensions">�g���q���X�g�i��: { ".png", ".jpg" }�j</param>
/// <returns>�t�B���^���ꂽ�t�@�C�����X�g</returns>
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
                break; // �ǂꂩ��v������ǉ����Ď���
            }
        }
    }

    return filtered;
}


/// <summary>
/// ���[�h�ς݂̂��ׂẴV�[���f�[�^��W���o�͂ɕ\������
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
/// �w�肵���V�[���̃f�[�^���폜����
/// </summary>
/// <param name="sceneName">�폜����V�[����</param>
void DataManager::RemoveSceneData(const std::string& sceneName)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sceneData_.erase(sceneName);

    char buf[256];
    snprintf(buf, sizeof(buf), "[INFO] Scene '%s' removed.", sceneName.c_str());
    LogMessage(buf);
}

/// <summary>
/// ���ׂẴV�[���f�[�^���폜����
/// </summary>
void DataManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(mutex_);
    sceneData_.clear();

    LogMessage("[INFO] All scenes cleared.");
}

#pragma once

#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <mutex>

// �V�[�����Ƃ̃f�[�^���Ǘ�����V���O���g��
class DataManager
{
public:
    // �V���O���g���i�����E�擾�E�폜�j
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new DataManager(); } };
    static DataManager* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }
private:
    // �f�t�H���g�R���X�g���N�^��private�ɂ��āA�O�����琶���ł��Ȃ��l�ɂ���
    DataManager(void);
    // �f�X�g���N�^�����l
    ~DataManager(void);

    // �R�s�[�E���[�u������֎~
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager(DataManager&&) = delete;
    DataManager& operator=(DataManager&&) = delete;

    // ���L���R���p�C���G���[�����邽�� ��L��ǉ�
    // DataManager copy = *DataManager::GetInstance();
    // DataManager copied(*DataManager::GetInstance());
    // DataManager moved = std::move(*DataManager::GetInstance());

public:
    /// <summary>
    /// �w�肵���V�[�����ƃt�H���_�p�X�����[�h����
    /// </summary>
    /// <param name="sceneName">�V�[�����i��: "TitleScene"�j</param>
    /// <param name="folderPath">���[�h����t�H���_�p�X</param>
    void LoadSceneData(const std::string& sceneName, const std::string& folderPath);

    /// <summary>
    /// �w��t�H���_���̂��ׂẴT�u�t�H���_���V�[���Ƃ��ă��[�h����
    /// </summary>
    /// <param name="rootPath">rootPath ��t�H���_�i��: "Data"�j</param>
    /// <param name="overwrite">overwrite ���ɑ��݂���V�[�������������ꍇ�ɏ㏑�����邩�i�f�t�H���gfalse�ŃX�L�b�v�j</param>
    void LoadAllData(const std::string& rootPath, bool overwrite = false);

    /// <summary>
    /// �V�[�����ƃt�H���_���Ńt�@�C���ꗗ���擾����
    /// </summary>
    /// <param name="sceneName">�V�[����</param>
    /// <param name="folderName">�t�H���_���i���΃p�X�j</param>
    /// <returns>�t�@�C���p�X�̃��X�g</returns>
    const std::vector<std::string>& GetFiles(const std::string& sceneName, const std::string& folderName) const;

    /// <summary>
    /// �����g���q�t�B���^�t���Ńt�@�C���ꗗ���擾����
    /// </summary>
    std::vector<std::string> GetFilesWithExtensions(
        const std::string& sceneName,
        const std::string& folderName,
        const std::vector<std::string>& extensions) const;

    /// <summary>
    /// ���[�h�ς݃f�[�^�����ׂĕ\��
    /// </summary>
    void PrintAll() const;

    /// <summary>
    /// �w�肵���V�[���̃f�[�^���폜����
    /// </summary>
    /// <param name="sceneName">�폜����V�[����</param>
    void RemoveSceneData(const std::string& sceneName);

    /// <summary>
    /// ���ׂẴV�[���f�[�^���폜����
    /// </summary>
    void ClearAll();

private:
    // �ÓI�C���X�^���X
    static DataManager* instance_;

    // �f�[�^�\��: �V�[���� -> (�t�H���_�� -> �t�@�C���p�X���X�g)
    std::map<std::string, std::map<std::string, std::vector<std::string>>> sceneData_;

    // �X���b�h���S�p
    mutable std::mutex mutex_;
};
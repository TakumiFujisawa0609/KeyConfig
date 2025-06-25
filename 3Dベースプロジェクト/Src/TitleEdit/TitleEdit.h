#pragma once
#include <vector>
#include <string>

class TitleEdit 
{
public:
    TitleEdit();
    ~TitleEdit();

    void Init();    // ������
    void Update();  // �X�V
    void Draw();    // �`��
    void Delete();  // �폜

    // �ۑ��E�ǂݍ���
    void SaveToCSV(const std::string& filename);
    void LoadFromCSV(const std::string& filename);
private:
    // Item�\����
    struct Item
    {
        std::string path;       // �t�@�C���p�X

        int handle = -1;        // �摜�n���h��
        int x = 0;              // X���W
        int y = 0;              // Y���W
        int width = 0;          // ����
        int height = 0;         // �c��
        float scale = 1.0f;     // �g��l
        float angle = 0.0f;     // ��]�l

        bool selected = false;  // �I���t���O
        bool used = true;       // �ۑ��t���O
    };
private:
    std::vector<Item> items_;   // �A�C�e���ێ��p

    // �A�C�e���ǉ�
    void AddNewItem(const std::string& path, int x, int y);

    // �S�Ă̑I���t���O��܂�
    void DeselectAll();
    
    // �}�E�X�̓����蔻��
    bool IsMouseOnItem(const Item& item, int mx, int my);
};
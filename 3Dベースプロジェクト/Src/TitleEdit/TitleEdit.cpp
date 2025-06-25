#include "TitleEdit.h"
#include "../DataManager/DataManager.h"
#include "../Input/InputManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "../Common/Debug.h"

TitleEdit::TitleEdit()
{
}

TitleEdit::~TitleEdit()
{
    // ハンドルの解放など必要に応じて
}

void TitleEdit::Init()
{
    // TitleScene データロード
    DataManager::GetInstance()->LoadSceneData("TitleScene", "Data/TitleScene");
    auto files = DataManager::GetInstance()->GetFiles("TitleScene", ".");

    int offset = 100;
    for (const auto& file : files) 
    {
        AddNewItem(file, offset, offset);
        offset += 50;
    }
}

void TitleEdit::Update()
{
    int mx, my;
    GetMousePoint(&mx, &my);

    if (InputManager::GetInstance()->IsTrgMouseLeft())
    {
        // 逆順でチェック（手前から奥へ）
        for (auto it = items_.rbegin(); it != items_.rend(); ++it)
        {
            // 範囲内？
            if (IsMouseOnItem(*it, mx, my))
            {
                // まず全部選択解除
                DeselectAll();

                // クリックされたのだけ選択
                it->selected = true;

                // 一番上を見つけたら終わり
                break;
            }
        }
    }

    // 選択中を移動
    if (InputManager::GetInstance()->IsClickMouseRight())
    {
        for (auto& item : items_)
        {
            if (item.selected)
            {
                item.x = mx;
                item.y = my;
            }
        }
    }

    // 選択中を回転・スケール
    for (auto& item : items_)
    {
        if (item.selected)
        {
            // 回転値を設定
            if (InputManager::GetInstance()->IsNew(KEY_INPUT_Q))
                item.angle -= 0.05f;
            if (InputManager::GetInstance()->IsNew(KEY_INPUT_E))
                item.angle += 0.05f;

            // 拡大値を設定
            if (InputManager::GetInstance()->IsNew(KEY_INPUT_Z))
            {
                item.scale -= 0.01f;
            }
            if (InputManager::GetInstance()->IsNew(KEY_INPUT_X))
            {
                item.scale += 0.01f;
            }

            if (item.scale < 0.1f) item.scale = 0.1f;
        }
    }

    // PageUp: 選択中を前（手前）に
    if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_W))
    {
        for (size_t i = 1; i < items_.size(); ++i) {
            if (items_[i].selected) {
                std::swap(items_[i], items_[i - 1]);
            }
        }
    }

    // PageDown: 選択中を後ろ（奥）に
    if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_S))
    {
        for (size_t i = items_.size() - 1; i > 0; --i) {
            if (items_[i - 1].selected) {
                std::swap(items_[i - 1], items_[i]);
            }
        }
    }

    // 選択中のアイテムの used フラグを切り替え（例: Uキー押下）
    if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_U))
    {
        for (auto& item : items_)
        {
            if (item.selected)
            {
                item.used = !item.used;
                break; // 複数選択できない場合は一つだけ切り替え
            }
        }
    }
}

void TitleEdit::Draw()
{
    for (const auto& item : items_)
    {
        if (item.handle == -1) continue;

        DrawRotaGraph2(
            item.x, item.y,
            item.width / 2, item.height / 2,
            item.scale,
            item.angle,
            item.handle,
            true
        );

        if (item.selected)
        {
            // 選択中のアイテムの枠の色を used 状態で変える
            unsigned int color = item.used ? GetColor(0, 255, 0) : GetColor(255, 0, 0);
            DrawCircle(item.x, item.y, 10, color, TRUE);
        }
    }

    DrawFormatString(10, 16, GetColor(255, 0, 0),
        "LMB: Select | RMB: Drag selected | Q/E: Rotate | Z/X: Scale | W/S: DrawOrder | U: Toggle Used | ESC: Exit");
}

void TitleEdit::Delete()
{
    // すべての画像ハンドルを解放
    for (auto& item : items_)
    {
        if (item.handle != -1)
        {
            DeleteGraph(item.handle);
            item.handle = -1;
        }
    }

    // vector を空にする
    items_.clear();
    items_.shrink_to_fit();
}

void TitleEdit::LoadFromCSV(const std::string& fileName)
{
    items_.clear();

    std::ifstream ifs(fileName);
    if (!ifs)
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "[ERROR] Failed to open file for loading：%s", fileName.c_str());
        LogMessage(buf);
        return;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue; // 空行スキップ

        std::istringstream iss(line);
        std::string token;

        Item item;

        // CSVの各要素を安全に読み込む
        if (!std::getline(iss, item.path, ',')) continue;
        if (!std::getline(iss, token, ',')) continue; item.x = std::stoi(token);
        if (!std::getline(iss, token, ',')) continue; item.y = std::stoi(token);
        if (!std::getline(iss, token, ',')) continue; item.scale = std::stof(token);
        if (!std::getline(iss, token, ',')) continue; item.angle = std::stof(token);

        item.handle = LoadGraph(item.path.c_str());
        if (item.handle != -1)
        {
            GetGraphSize(item.handle, &item.width, &item.height);
        }
        else
        {
            // ロード失敗時のログ
            char buf[512];
            snprintf(buf, sizeof(buf), "[ERROR] Failed to load image：%s", item.path.c_str());
            LogMessage(buf);
            continue;
        }

        items_.push_back(item);
    }
}

void TitleEdit::SaveToCSV(const std::string& fileName)
{
    std::ofstream ofs(fileName);
    if (!ofs)
    {
        // ファイルが開けなかった場合はエラーメッセージ
        char buf[512];
        snprintf(buf, sizeof(buf), "[ERROR] Failed to open file for saving：%s", fileName.c_str());
        LogMessage(buf);
        return;
    }

    for (const auto& item : items_)
    {
        if (!item.used) continue;  // used = falseは保存しない

        ofs << item.path << ","
            << item.x << ","
            << item.y << ","
            << item.scale << ","
            << item.angle << "\n";
    }
}

void TitleEdit::AddNewItem(const std::string& path, int x, int y)
{
    Item item;
    item.path = path;
    item.handle = LoadGraph(item.path.c_str());
    
    if (item.handle != -1)
    {
        GetGraphSize(item.handle, &item.width, &item.height);
        item.scale = 1.0f;
    }

    item.x = x;
    item.y = y;

    item.angle = 0.0f;
    item.selected = false;
    
    items_.push_back(item);
}

void TitleEdit::DeselectAll()
{
    for (auto& item : items_)
    {
        item.selected = false;
    }
}

bool TitleEdit::IsMouseOnItem(const Item& item, int mx, int my)
{
    // アイテムの中心基準にする
    float dx = mx - item.x;
    float dy = my - item.y;

    // アイテムの逆回転をマウス座標に適用
    float cosA = cos(-item.angle);
    float sinA = sin(-item.angle);

    float localX = cosA * dx - sinA * dy;
    float localY = sinA * dx + cosA * dy;

    // スケール考慮
    float halfWidth = (item.width * item.scale) / 2.0f;
    float halfHeight = (item.height * item.scale) / 2.0f;

    return (localX >= -halfWidth && localX <= halfWidth &&
        localY >= -halfHeight && localY <= halfHeight);
}

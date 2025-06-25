#pragma once
#include <vector>
#include <string>

class TitleEdit 
{
public:
    TitleEdit();
    ~TitleEdit();

    void Init();    // 初期化
    void Update();  // 更新
    void Draw();    // 描画
    void Delete();  // 削除

    // 保存・読み込み
    void SaveToCSV(const std::string& filename);
    void LoadFromCSV(const std::string& filename);
private:
    // Item構造体
    struct Item
    {
        std::string path;       // ファイルパス

        int handle = -1;        // 画像ハンドル
        int x = 0;              // X座標
        int y = 0;              // Y座標
        int width = 0;          // 横幅
        int height = 0;         // 縦幅
        float scale = 1.0f;     // 拡大値
        float angle = 0.0f;     // 回転値

        bool selected = false;  // 選択フラグ
        bool used = true;       // 保存フラグ
    };
private:
    std::vector<Item> items_;   // アイテム保持用

    // アイテム追加
    void AddNewItem(const std::string& path, int x, int y);

    // 全ての選択フラグを折る
    void DeselectAll();
    
    // マウスの当たり判定
    bool IsMouseOnItem(const Item& item, int mx, int my);
};
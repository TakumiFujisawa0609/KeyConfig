#pragma once
#include <DxLib.h>
#include <cmath>

/// <summary>
/// 四角形と点の当たり判定
/// </summary>
/// <param name="RectX">四角形の座標X</param>
/// <param name="RectY">四角形の座標Y</param>
/// <param name="RectW">四角形の横幅</param>
/// <param name="RectH">四角形の縦幅</param>
/// <param name="mousePosX">マウスの座標X</param>
/// <param name="mousePosY">マウスの座標Y</param>
/// <returns>true = 当たっている</returns>
static bool CollCheckPointRect(int RectX, int RectY, int RectW, int RectH, int mousePosX, int mousePosY)
{
    return (RectX - (RectW / 2) < mousePosX &&
        RectX - (RectW / 2) + RectW > mousePosX &&
        RectY - (RectH / 2) < mousePosY &&
        RectY - (RectH / 2) + RectH > mousePosY);
}

/// <summary>
/// 3D四角形同士の当たり判定
/// </summary>
/// <param name="boxAPos">四角形Aの座標</param>
/// <param name="boxASize">四角形Aのサイズ</param>
/// <param name="boxBPos">四角形Bの座標</param>
/// <param name="boxBSize">四角形Aのサイズ</param>
/// <returns>true = 当たっている</returns>
static bool CollCheckCube(VECTOR boxAPos, VECTOR boxASize, VECTOR boxBPos, VECTOR boxBSize)
{
    return (boxAPos.x < boxBPos.x + boxBSize.x &&
        boxAPos.x + boxASize.x > boxBPos.x &&
        boxAPos.y < boxBPos.y + boxBSize.y &&
        boxAPos.y + boxASize.y > boxBPos.y &&
        boxAPos.z < boxBPos.z + boxBSize.z &&
        boxAPos.z + boxASize.z > boxBPos.z
        );
}

static bool CollCheckCubeCenter(VECTOR boxAPos, VECTOR boxASize, VECTOR boxBPos, VECTOR boxBSize)
{
    return (abs(boxAPos.x - boxBPos.x) < (boxASize.x / 2 + boxBSize.x / 2) &&
        abs(boxAPos.y - boxBPos.y) < (boxASize.y / 2 + boxBSize.y / 2) &&
        abs(boxAPos.z - boxBPos.z) < (boxASize.z / 2 + boxBSize.z / 2));
}

/// <summary>
/// 3D球同士の当たり判定
/// </summary>
/// <param name="sphereAPos">球体Aの座標</param>
/// <param name="sphereARadius">球体Bの半径</param>
/// <param name="sphereBPos">球体Bの座標</param>
/// <param name="sphereBRadius">球体Bの半径</param>
/// <returns>true = 当たっている</returns>
static bool CollCheckSphere(VECTOR sphereAPos, float sphereARadius, VECTOR sphereBPos, float sphereBRadius)
{
    // ２つの座標間の距離をピタゴラスの定理で算出
    VECTOR distance;
    distance.x = sphereAPos.x - sphereBPos.x;
    distance.y = sphereAPos.y - sphereBPos.y;
    distance.z = sphereAPos.z - sphereBPos.z;
    float dis = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;

    // お互いの半径を合計する
    float radius = sphereARadius + sphereBRadius;

    // 合計した半径の２乗よりも、２つの座標間の距離が小さければ球体は衝突している
    return (radius * radius > dis);
}

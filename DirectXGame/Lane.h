#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <PrimitiveDrawer.h>
#include "Model.h"

class Lane
{
public:

    Lane();
    ~Lane();

    struct LaneData
    {
        Vector3 left[2];    // !< 左レーン始点と終点
        Vector3 right[2];   // !< 右レーン始点と終点
        Vector3 direction;  // !< レーンの前方向（正規化必須）
        float   length;     // !< レーンの長さ
        float   interval;   // !< 右レーンと左レーンの間隔
    };

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw3D(const ViewProjection& _viewProjection);

    LaneData* GetLaneData() { return &laneData_; };

private:
    WorldTransform      worldTransform_;
    LaneData            laneData_;
    PrimitiveDrawer*    pPrimitiveDrawer_;
};